//
// Created by Matthew Good on 27/8/21.
//

#include "TextView.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wshadow"
#define IMGUI_DEFINE_MATH_OPERATORS

#include <imgui_internal.h>

bool TextView::onTouchEvent(MultiTouch &touch) {
    touchCopy = touch;
    return true;
}

void TextView::onResize() {
}

void TextView::beforeFrame() {
    ImGuiIO & io = ImGui::GetIO();

    // imgui does not appear to have an array of MousePos
    // which suggests it is unable to track multiple touches
    // however imgui has hardcoded 5 slot arrays of mouse data
    //
    // since MousePos is not an array, only track the first touch target

    auto it = touchCopy.getIterator();

    // we may not have received a touch yet
    if (it.hasNext()) {
        auto * touchData = it.next();

        io.MousePosPrev = io.MousePos;
        io.MousePos = {touchData->x, touchData->y};

        if (touchData->state == MultiTouch::TOUCH_DOWN) {
            io.MouseReleased[0] = false;
            io.MouseClicked[0] = true;
            io.MouseDown[0] = ImGuiMouseButton_Left;
        }

        if (touchData->state == MultiTouch::TOUCH_DOWN) {
            io.MouseClicked[0] = false;
            io.MouseReleased[0] = true;
            // imgui does not seem to have a MouseUp
        }
    }

    if (once) {
        font_size_to_set = fontSize_Load;
        cache();
        once = false;
    }

    if (needsFontSet.load()) {
        setFontInternal(font.load());
        needsFontSet.store(false);
    }

    bool needsRecalc = false;
    if (cache_displaySize.x != io.DisplaySize.x || cache_displaySize.y != io.DisplaySize.y) {
        cache_displaySize.x = io.DisplaySize.x;
        cache_displaySize.y = io.DisplaySize.y;
        needsRecalc = true;
    }

    if (cache_text != text) {
        cache_text = text;
        needsRecalc = true;
    }

    if (font_size_change.load()) {
        ImGui::GetCurrentContext()->FontSize = font_size_to_set;
        needsRecalc = true;
        font_size_change.store(false);
    }

    if (needsRecalc) {
        if (textResizeMode == none) {
            cache();
        } else {
            computeFontSize(text.c_str());
        }
        computed_font_size = ImGui::GetCurrentContext()->FontSize;
    }
}

void TextView::onDraw() {
    if (font.load() == nullptr || needsFontSet.load()) return;
    ImGuiIO & io = ImGui::GetIO();

    ImGui::SetNextWindowPos({0.0f, 0.0f});
    ImGui::SetNextWindowSize(io.DisplaySize);

    ImVec2 box = {1, 0};
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, box);
    ImGui::PushStyleColor(ImGuiCol_Text, textColor.to_RGBA_unsigned_32bit_int());

    ImGui::Begin("TextView", nullptr,
                 ImGuiWindowFlags_::ImGuiWindowFlags_NoNav
                 | ImGuiWindowFlags_::ImGuiWindowFlags_NoMove
                 | ImGuiWindowFlags_::ImGuiWindowFlags_NoResize
                 | ImGuiWindowFlags_::ImGuiWindowFlags_NoNavInputs
                 | ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse
                 | ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollbar
                 | ImGuiWindowFlags_::ImGuiWindowFlags_NoBackground
                 | ImGuiWindowFlags_::ImGuiWindowFlags_NoDecoration
                 | ImGuiWindowFlags_::ImGuiWindowFlags_NoSavedSettings
                 | ImGuiWindowFlags_::ImGuiWindowFlags_AlwaysAutoResize
    );

    ImGui::PushClipRect({1, 0}, {io.DisplaySize.x-1, io.DisplaySize.y}, true);

    ImGui::PushTextWrapPos(-1.0f);

    // the font size is set to the loaded font size
    // when the window is created by Imgui::Begin
    // restore it

    ImGui::GetCurrentContext()->FontSize = computed_font_size;

    ImVec2 bottomRight;

    for (size_t i = 1; i <= text.size(); i++) {
        try {
            // text: abc
            // i = 1, substr = a
            // i = 2, substr = ab
            // i = 3, substr = abc
            std::string substr = text.substr(0, i);
            bottomRight = ImGui::CalcTextSize(substr.c_str());
            ImGui::GetWindowDrawList()->AddRect(box, bottomRight, boundingBoxCharacterColor.to_RGBA_unsigned_32bit_int());
        } catch(const std::out_of_range& e) {
            Log::Error_And_Throw("pos ", i, " exceeds string size ", text.size());
        }
    }

    bottomRight = ImGui::CalcTextSize(text.c_str());
    ImGui::GetWindowDrawList()->AddRect(box, bottomRight, boundingBoxColor.to_RGBA_unsigned_32bit_int());

    ImGui::Text("%s", text.c_str());

    ImGui::PopTextWrapPos();

    ImGui::PopClipRect();

    ImGui::End();

    // the font size is set to the loaded font size
    // when the window is popped by Imgui::End
    // restore it

    ImGui::GetCurrentContext()->FontSize = computed_font_size;

    ImGui::PopStyleColor();
    ImGui::PopStyleVar();
}

void TextView::computeFontSize(const char * text) {
    ImGuiIO & io = ImGui::GetIO();

    ImVec2 textSize = ImGui::CalcTextSize(text);

    // if we are larger than our text view, incrementally try smaller sizes until we fit
    while (textSize.y > io.DisplaySize.y) {
        ImGui::GetCurrentContext()->FontSize -= 1;
        cache();
        textSize = ImGui::CalcTextSize(text);
    }
    // if we are smaller than our text view, incrementally try larger sizes until we fit
    while (textSize.y < io.DisplaySize.y) {
        ImGui::GetCurrentContext()->FontSize += 1;
        cache();
        textSize = ImGui::CalcTextSize(text);
    }
}

float TextView::getUpperBound(std::vector<float> & vec, float input) {
    const std::vector<float>::iterator & begin = vec.begin();
    const std::vector<float>::iterator & end = vec.end();
    auto n = std::lower_bound(begin, end, input);
    if (n == end) {
        n = end-1;
    }
    return *n;
};

float TextView::getLowerBound(std::vector<float> & vec, float input) {
    const std::vector<float>::reverse_iterator & begin = vec.rbegin();
    const std::vector<float>::reverse_iterator & end = vec.rend();
    auto n = std::lower_bound(begin, end, input, std::greater<float>());
    if (n == end) {
        n = end-1;
    }
    return *n;
};

void TextView::cache() {
    size_t size = values.size();
    if (size < 2) {
        Log::Error_And_Throw("values must contain at least 2 elements");
    }

    float min = values[0];
    float max = values[size-1];

    CacheDirection direction;

    // font size
    float font_size = ImGui::GetCurrentContext()->FontSize;
    if (cache_font_size != font_size) {
        direction = font_size < cache_font_size ? down : up;
        cache_direction = direction;
        cache_font_size = font_size;
    } else {
        direction = cache_direction;
    }

    float font_load_size_upper = getUpperBound(values, font_size);
    float font_load_size_lower = getLowerBound(values, font_size);

    if (font_load_size_lower == font_load_size_upper) {
        if (direction == up) {
            if (font_load_size_upper != max) {
                font_load_size_upper = getUpperBound(values, font_size+1);
            }
        } else {
            if (font_load_size_lower != min) {
                font_load_size_lower = getLowerBound(values, font_size-1);
            }
        }
    } else {
        if (direction == down && font_load_size_lower == font_size) {
            font_load_size_lower = getLowerBound(values, font_size-1);
        }
    }

    bool cache_set = false;

    if (cache_fontSize_Upper != font_load_size_upper || cache_fontSize_Lower != font_load_size_lower) {
        cache_fontSize_Upper = font_load_size_upper;
        cache_fontSize_Lower = font_load_size_lower;
        cache_set = true;
    }

    if (log_cache) {
        Log::Info("upper: ", font_load_size_upper);
        Log::Info("lower: ", font_load_size_lower);
        Log::Info("cache: ", cache_fontSize_Load);
        Log::Info("cache upper: ", cache_fontSize_Upper);
        Log::Info("cache lower: ", cache_fontSize_Lower);
        Log::Info("direction: ", direction == up ? "up" : "down");
    }

    if (cache_set) {
        cache_fontSize_Load = font_load_size_upper;
    }

    if (cache_set) {
        // as an optimization, the text is upscaled by a factor of 2
        // this means if the text size is 400
        // then the text size is 200 and then upscaled to 400
        //
        // this can greatly reduces memory
        // due to fonts loading a large number of glyphs

        if (log_font_loading) {
            Log::Info("loading font at size ", cache_fontSize_Load / fontSizeUpscaleFactor);
        }

        // load texture atlas at desired font size
        fontSize_Load = cache_fontSize_Load / fontSizeUpscaleFactor;
        setFontDefault();
        generateFontTextures();
        setFontInternal(font.load());
        needsFontSet.store(false);
        ImGui::GetCurrentContext()->FontSize = font_size;
        min_loaded = cache_fontSize_Load == min;
    }
}

TextView::TextResizeMode TextView::getTextResizeMode() const {
    return textResizeMode;
}

void TextView::setTextResizeMode(TextView::TextResizeMode textResizeMode) {
    TextView::textResizeMode = textResizeMode;
}

float TextView::getFontSize() const {
    return ImGui::GetCurrentContext()->FontSize;
}

void TextView::setFontSize(float fontSize) {
    font_size_to_set = fontSize;
    font_size_change.store(true);
}

const std::string &TextView::getText() const {
    return text;
}

void TextView::setText(const std::string &text) {
    TextView::text = text;
}

const VertexEngine::Color4 &TextView::getTextColor() const {
    return textColor;
}

void TextView::setTextColor(const VertexEngine::Color4 &textColor) {
    TextView::textColor = textColor;
}

ImFont *TextView::getFont() const {
    return font.load();
}

void TextView::setFont(ImFont *font) {
    TextView::font.store(font);
    needsFontSet.store(true);
}

void TextView::setFontDefault(const ImFontConfig *font_cfg_template) {
    auto * default_font_cfg = ImGui::GetIO().Fonts->Fonts.Data[0]->ConfigData;
    ImFontConfig font_cfg;
    font_cfg.OversampleH = default_font_cfg->OversampleH;
    font_cfg.OversampleV = default_font_cfg->OversampleV;
    font_cfg.PixelSnapH = default_font_cfg->PixelSnapH;
    font_cfg.SizePixels = fontSize_Load;
    void * FontData = IM_ALLOC(default_font_cfg->FontDataSize);
    font_cfg.FontDataOwnedByAtlas = true;
    memcpy(FontData, default_font_cfg->FontData, (size_t)default_font_cfg->FontDataSize);
    font_cfg.MergeMode = false;
    ImFormatString(font_cfg.Name, IM_ARRAYSIZE(font_cfg.Name), "ProggyClean.ttf, %dpx", (int)font_cfg.SizePixels);
    font_cfg.EllipsisChar = default_font_cfg->EllipsisChar;
    font_cfg.GlyphOffset.y = 1;

    setFontFromMemoryTTF(
                         FontData,
                         default_font_cfg->FontDataSize,
                         font_cfg.SizePixels, &font_cfg,
                         default_font_cfg->GlyphRanges
    );
}

void TextView::setFontFromFileTTF(const char *filename, float size_pixels,
                                   const ImFontConfig *font_cfg_template,
                                   const ImWchar *glyph_ranges) {
    auto & io = ImGui::GetIO();
    io.Fonts->Clear();
    setFont(io.Fonts->AddFontFromFileTTF(filename, size_pixels, font_cfg_template, glyph_ranges));
}

void TextView::setFontFromMemoryTTF(void *ttf_data, int ttf_size, float size_pixels,
                                     const ImFontConfig *font_cfg_template,
                                     const ImWchar *glyph_ranges) {
    auto & io = ImGui::GetIO();
    io.Fonts->Clear();
    setFont(io.Fonts->AddFontFromMemoryTTF(ttf_data, ttf_size, size_pixels, font_cfg_template, glyph_ranges));
}

void
TextView::setFontFromMemoryCompressedTTF(const void *compressed_ttf_data, int compressed_ttf_size,
                                          float size_pixels, const ImFontConfig *font_cfg_template,
                                          const ImWchar *glyph_ranges) {
    auto & io = ImGui::GetIO();
    io.Fonts->Clear();
    setFont(io.Fonts->AddFontFromMemoryCompressedTTF(compressed_ttf_data, compressed_ttf_size, size_pixels, font_cfg_template, glyph_ranges));
}

void TextView::setFontFromMemoryCompressedBase85TTF(const char *compressed_ttf_data_base85,
                                                     float size_pixels,
                                                     const ImFontConfig *font_cfg,
                                                     const ImWchar *glyph_ranges) {
    auto & io = ImGui::GetIO();
    io.Fonts->Clear();
    setFont(io.Fonts->AddFontFromMemoryCompressedBase85TTF(compressed_ttf_data_base85, size_pixels, font_cfg, glyph_ranges));
}

void TextView::setFontInternal(ImFont *pFont) {
    ImGui::SetCurrentFont(pFont);
    // the font size is only set if we have a window,
    // however the font cannot be loaded inside a frame
    // so we need to manually set the font size
    ImGui::GetCurrentContext()->FontSize = pFont->FontSize;
}

#pragma clang diagnostic pop
