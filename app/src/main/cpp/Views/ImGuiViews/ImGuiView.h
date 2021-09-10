//
// Created by Matthew Good on 27/8/21.
//

#ifndef GRAPHICAL_TOOL_KIT_IMGUIVIEW_H
#define GRAPHICAL_TOOL_KIT_IMGUIVIEW_H

#include <Views/View.h>
#include <imgui.h>

/**
 * an empty ImGui View, this by itself does nothing
 */
class ImGuiView : public View {
    Diligent::ImGuiImplDiligent * m_pImGui = nullptr;
    
    RenderTarget rt;
    static constexpr const char * PIPELINE_KEY = "ImGuiView RT";
    
    PipelineManager * resizePipeline;
    
public:
    ImGuiContext * imgui_context = nullptr;
    ImGuiIO * imgui_io = nullptr;

    void createPipeline(PipelineManager &pipelineManager) override final;

    void switchToPipeline(PipelineManager &pipelineManager) override final;

    void bindShaderResources(PipelineManager &pipelineManager) override final;

    void destroyPipeline(PipelineManager &pipelineManager) override final;

    void create() override final;

    void resize(PipelineManager &pipelineManager) override final;

    void draw(DrawTools &drawTools, RenderTarget &screenRenderTarget,
              RenderTarget &renderTarget) override final;

    void destroy() override final;

    virtual void onCreate();
    virtual void onResize();
    virtual void onUpdateFontTextures();
    void generateFontTextures();
    virtual void beforeFrame();
    virtual void onDraw();
    virtual void onDestroy();
};

#endif //GRAPHICAL_TOOL_KIT_IMGUIVIEW_H
