//
// Created by Matthew Good on 27/8/21.
//

#ifndef GRAPHICAL_TOOL_KIT_TEXTVIEW_H
#define GRAPHICAL_TOOL_KIT_TEXTVIEW_H

#include "ImGuiView.h"

class TextView : public ImGuiView {
public:
    TextView();
    TextView(const std::string &text);

private:
    MultiTouch touchCopy;

    ImVec2 cache_displaySize {0, 0};

    std::atomic<ImFont *> font = {nullptr};
    std::atomic_bool font_size_change = {false};
    std::atomic_bool needsFontSet {false};
    
    bool log_cache = false;
    bool log_font_loading = false;
    
    float fontSize_Load = 13;
    float font_size_to_set = 13;
    float cache_font_size = 0;
    float computed_font_size = 0;
    
    bool min_loaded = false;

    float cache_fontSize_Load = 0;
    float cache_fontSize_Upper = 0;
    float cache_fontSize_Lower = 0;
    
    float getUpperBound(std::vector<float> & vec, float input);
    float getLowerBound(std::vector<float> & vec, float input);

    float fontSizeUpscaleFactor = 2;
    
    std::vector<float> values = {256, 512, 1024};

    std::string cache_text = "TextView";
    std::string text = "TextView";
    
    VertexEngine::Color4 textColor = VertexEngine::Color4(0,1,0,1);
    VertexEngine::Color4 boundingBoxCharacterColor = VertexEngine::Color4(0,0,1,1);
    VertexEngine::Color4 boundingBoxColor = VertexEngine::Color4(1,0,1,1);

public:
    enum TextResizeMode {
        none,
        height
    };
    
    enum CacheDirection {
        up, down
    };
    
private:
    CacheDirection cache_direction;

    TextView * cache();
    
    TextResizeMode textResizeMode = TextResizeMode::none;

    TextView * computeFontSize(const Rectangle &dimensions, const char * text);

    TextView * drawBoundings();

    TextView * drawText();

public:
    void beforeFrame() override;

    TextResizeMode getTextResizeMode() const;

    TextView * setTextResizeMode(TextResizeMode textResizeMode);

    ImFont *getFont() const;
    TextView * setFont(ImFont *font);

    TextView * setFontDefault(const ImFontConfig* font_cfg = NULL);
    TextView * setFontFromFileTTF(const char* filename, float size_pixels, const ImFontConfig* font_cfg = NULL, const ImWchar* glyph_ranges = NULL);
    TextView * setFontFromMemoryTTF(void* font_data, int font_size, float size_pixels, const ImFontConfig* font_cfg = NULL, const ImWchar* glyph_ranges = NULL); // Note: Transfer ownership of 'ttf_data' to ImFontAtlas! Will be deleted after destruction of the atlas. Set font_cfg->FontDataOwnedByAtlas=false to keep ownership of your data and it won't be freed.
    TextView * setFontFromMemoryCompressedTTF(const void* compressed_font_data, int compressed_font_size, float size_pixels, const ImFontConfig* font_cfg = NULL, const ImWchar* glyph_ranges = NULL); // 'compressed_font_data' still owned by caller. Compress with binary_to_compressed_c.cpp.
    TextView * setFontFromMemoryCompressedBase85TTF(const char* compressed_font_data_base85, float size_pixels, const ImFontConfig* font_cfg = NULL, const ImWchar* glyph_ranges = NULL);              // 'compressed_font_data_base85' still owned by caller. Compress with binary_to_compressed_c.cpp with -base85 parameter.

    float getFontSize() const;

    TextView * setFontSize(float fontSize);

    const std::string &getText() const;

    TextView * setText(const std::string &text);

    const VertexEngine::Color4 &getTextColor() const;

    TextView * setTextColor(const VertexEngine::Color4 &textColor);

    void onDraw() override;

    bool onTouchEvent(MultiTouch &touch) override;

    TextView * setFontInternal(ImFont *pFont);

    void onMeasure() override;

    void onLayout(bool changed, const Rectangle &dimensions, DrawTools &drawTools, RenderTarget &screenRenderTarget, RenderTarget &renderTarget) override;
};


#endif //GRAPHICAL_TOOL_KIT_TEXTVIEW_H
