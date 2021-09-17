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

void TextView::beforeFrame() {

    // imgui does not appear to have an array of MousePos
    // which suggests it is unable to track multiple touches
    // however imgui has hardcoded 5 slot arrays of mouse data
    //
    // since MousePos is not an array, only track the first touch target

    auto it = touchCopy.getIterator();

    // we may not have received a touch yet
    if (it.hasNext()) {
        auto * touchData = it.next();

        imgui_io->MousePosPrev = imgui_io->MousePos;
        imgui_io->MousePos = {touchData->x, touchData->y};

        if (touchData->state == MultiTouch::TOUCH_DOWN) {
            imgui_io->MouseReleased[0] = false;
            imgui_io->MouseClicked[0] = true;
            imgui_io->MouseDown[0] = ImGuiMouseButton_Left;
        }

        if (touchData->state == MultiTouch::TOUCH_DOWN) {
            imgui_io->MouseClicked[0] = false;
            imgui_io->MouseReleased[0] = true;
            // imgui does not seem to have a MouseUp
        }
    }
}

void TextView::onCreate() {
    setFontSize(fontSize_Load);
}

void TextView::onMeasure() {
    if (needsFontSet.load()) {
        setFontInternal(font.load());
        needsFontSet.store(false);
    }

    bool needsRecalc = false;
    if (cache_displaySize.x != imgui_io->DisplaySize.x || cache_displaySize.y != imgui_io->DisplaySize.y) {
        cache_displaySize.x = imgui_io->DisplaySize.x;
        cache_displaySize.y = imgui_io->DisplaySize.y;
        needsRecalc = true;
    }

    if (cache_text != text) {
        cache_text = text;
        needsRecalc = true;
    }

    if (font_size_change.load()) {
        imgui_context->FontSize = font_size_to_set;
        needsRecalc = true;
        font_size_change.store(false);
    }

    if (needsRecalc) {
        cache();
        computed_font_size = imgui_context->FontSize;
    }

    setMeasuredDimensions(MeasureSpec::MATCH_PARENT, 13);
}

void TextView::onLayout(bool changed, const Rectangle &dimensions, DrawTools &drawTools, RenderTarget &screenRenderTarget, RenderTarget &renderTarget) {
    if (textResizeMode == none) {
        cache();
    } else {
        computeFontSize(dimensions, text.c_str());
    }
    computed_font_size = imgui_context->FontSize;
}

void TextView::drawBoundings() {
    ImGui::SetNextWindowPos({0, 0});
    ImGui::SetNextWindowSize(imgui_io->DisplaySize);

    ImVec2 box = {1, 1};
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, box);

    ImGui::Begin("TextView Boundings", nullptr,
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

    // the font size is set to the loaded font size
    // when the window is created by Imgui::Begin
    // restore it

    imgui_context->FontSize = computed_font_size;
    
    ImVec2 bottomRight;

    for (size_t i = 1; i <= text.size(); i++) {
        try {
            // text: abc
            // i = 1, substr = a
            // i = 2, substr = ab
            // i = 3, substr = abc
            std::string substr = text.substr(0, i);
            bottomRight = ImGui::CalcTextSize(substr.c_str(), nullptr, false, -1.0f);
            ImGui::GetWindowDrawList()->AddRect(box, {bottomRight.x+1, bottomRight.y - 1}, boundingBoxCharacterColor.to_RGBA_unsigned_32bit_int());
        } catch(const std::out_of_range& e) {
            Log::Error_And_Throw("pos ", i, " exceeds string size ", text.size());
        }
    }

    // set bounding box to wrapped text bounds
    bottomRight = ImGui::CalcTextSize(text.c_str());
    ImGui::GetWindowDrawList()->AddRect(box, {bottomRight.x+1, bottomRight.y - 1}, boundingBoxColor.to_RGBA_unsigned_32bit_int());

    ImGui::End();

    // the font size is set to the loaded font size
    // when the window is popped by Imgui::End
    // restore it

    imgui_context->FontSize = computed_font_size;

    ImGui::PopStyleVar();
}

void TextView::drawText() {
    ImGui::SetNextWindowPos({0, 0});
    ImGui::SetNextWindowSize(imgui_io->DisplaySize);

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

    ImGui::PushTextWrapPos(-1.0f);

    // the font size is set to the loaded font size
    // when the window is created by Imgui::Begin
    // restore it

    imgui_context->FontSize = computed_font_size;

    ImGui::Text("%s", text.c_str());

    ImGui::PopTextWrapPos();

    ImGui::End();

    // the font size is set to the loaded font size
    // when the window is popped by Imgui::End
    // restore it

    imgui_context->FontSize = computed_font_size;

    ImGui::PopStyleColor();
    ImGui::PopStyleVar();
}

void TextView::onDraw() {
    if (font.load() == nullptr || needsFontSet.load()) return;
    
    drawBoundings();
    drawText();
}

void TextView::computeFontSize(const Rectangle &dimensions, const char * text) {
    // it seems that, in ImGui
    // assuming no text wrapping,
    // textSize.y is equal to the font size

    if (textResizeMode == height) {
        // optimization - we know that textSize.y is equal to font size
        //
        // this is only true when wrap is less than 0.0f
        //
        // avoid unnessisary font loading

        // since we know that the font size is directly equal to display height
        // then if we are not wrapping text we can directly set the height

        imgui_context->FontSize = dimensions.bottomRight.y;
        cache();
    } else {
        ImVec2 textSize = ImGui::CalcTextSize(text, nullptr, false, -1.0f);

        // if we are larger than our text view, incrementally try smaller sizes until we fit
        while (textSize.y > dimensions.bottomRight.y) {
            imgui_context->FontSize -= 1;
            cache();
            textSize = ImGui::CalcTextSize(text, nullptr, false, -1.0f);
        }
        // if we are smaller than our text view, incrementally try larger sizes until we fit
        while (textSize.y < dimensions.bottomRight.y) {
            imgui_context->FontSize += 1;
            cache();
            textSize = ImGui::CalcTextSize(text, nullptr, false, -1.0f);
        }
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
    float font_size = imgui_context->FontSize;
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
        imgui_context->FontSize = font_size;
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
    return imgui_context->FontSize;
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
    auto * default_font_cfg = imgui_io->Fonts->Fonts.Data[0]->ConfigData;
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
    imgui_io->Fonts->Clear();
    setFont(imgui_io->Fonts->AddFontFromFileTTF(filename, size_pixels, font_cfg_template, glyph_ranges));
}

void TextView::setFontFromMemoryTTF(void *ttf_data, int ttf_size, float size_pixels,
                                     const ImFontConfig *font_cfg_template,
                                     const ImWchar *glyph_ranges) {
    imgui_io->Fonts->Clear();
    setFont(imgui_io->Fonts->AddFontFromMemoryTTF(ttf_data, ttf_size, size_pixels, font_cfg_template, glyph_ranges));
}

void
TextView::setFontFromMemoryCompressedTTF(const void *compressed_ttf_data, int compressed_ttf_size,
                                          float size_pixels, const ImFontConfig *font_cfg_template,
                                          const ImWchar *glyph_ranges) {
    imgui_io->Fonts->Clear();
    setFont(imgui_io->Fonts->AddFontFromMemoryCompressedTTF(compressed_ttf_data, compressed_ttf_size, size_pixels, font_cfg_template, glyph_ranges));
}

void TextView::setFontFromMemoryCompressedBase85TTF(const char *compressed_ttf_data_base85,
                                                     float size_pixels,
                                                     const ImFontConfig *font_cfg,
                                                     const ImWchar *glyph_ranges) {
    imgui_io->Fonts->Clear();
    setFont(imgui_io->Fonts->AddFontFromMemoryCompressedBase85TTF(compressed_ttf_data_base85, size_pixels, font_cfg, glyph_ranges));
}

void TextView::setFontInternal(ImFont *pFont) {
    ImGui::SetCurrentFont(pFont);
    // the font size is only set if we have a window,
    // however the font cannot be loaded inside a frame
    // so we need to manually set the font size
    imgui_context->FontSize = pFont->FontSize;
}

TextView::TextView() {
    setFontSize(fontSize_Load);
}

TextView::TextView(const std::string &text) : text(text) {
    setFontSize(fontSize_Load);
}

#pragma clang diagnostic pop
