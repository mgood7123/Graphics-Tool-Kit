//
// Created by Matthew Good on 27/8/21.
//

#include "ImGuiView.h"

void ImGuiView::create() {
    m_pImGui = new Diligent::ImGuiImplDiligent(
            getDiligentAppBase().m_pDevice,
            getDiligentAppBase().m_pSwapChain->GetDesc().ColorBufferFormat,
            getDiligentAppBase().m_pSwapChain->GetDesc().DepthBufferFormat
    );
    m_pImGui->CreateDeviceObjects();
    imgui_context = ImGui::GetCurrentContext();
    imgui_io = &ImGui::GetIO();
    onCreate();
}

void ImGuiView::resize(PipelineManager &pipelineManager) {
    resizePipeline = &pipelineManager;
}

void ImGuiView::generateFontTextures() {
    m_pImGui->UpdateFontsTexture();
}

void
ImGuiView::draw(DrawTools &drawTools, RenderTarget &screenRenderTarget, RenderTarget &renderTarget) {

    auto & app = getDiligentAppBase();
    
    if (layoutData.bottomRight.x == 0 || layoutData.bottomRight.y == 0) {
        return;
    }

    imgui_io->DisplaySize = {
        layoutData.bottomRight.x,
        layoutData.bottomRight.y
    };
    
    beforeFrame();

    m_pImGui->NewFrame(
        layoutData.bottomRight.x,
        layoutData.bottomRight.y,
        app.m_pSwapChain->GetDesc().PreTransform
    );

    onDraw();

    rt.resize(*resizePipeline, layoutData.bottomRight.x, layoutData.bottomRight.y, app.m_pSwapChain, app.m_pDevice);
    rt.bind(app.m_pImmediateContext);
    rt.clearColorAndDepth(
        RenderTarget::black, 1.0f, app.m_pImmediateContext
    );
    
    m_pImGui->Render(app.m_pImmediateContext);

    renderTarget.bind(app.m_pImmediateContext);
    renderTarget.clearColorAndDepth(
        RenderTarget::black, 1.0f, app.m_pImmediateContext
    );

    drawTools.pixelToNDC.resize(renderTarget.getWidth(), renderTarget.getHeight());
    renderTarget.clipAbsolutePosition({0, 0, renderTarget.getWidth(), renderTarget.getHeight()}, app.m_pImmediateContext);
    rt.drawAbsolutePosition(drawTools, {0, 0, renderTarget.getWidth(), renderTarget.getHeight()}, app.m_pImmediateContext);
}

void ImGuiView::destroy() {
    onDestroy();
    m_pImGui->InvalidateDeviceObjects();
    imgui_io = nullptr;
    imgui_context = nullptr;
    delete m_pImGui;
}

void ImGuiView::onCreate() {

}

void ImGuiView::onResize() {

}

void ImGuiView::onUpdateFontTextures() {

}

void ImGuiView::beforeFrame() {

}

void ImGuiView::onDraw() {

}

void ImGuiView::onDestroy() {

}

void ImGuiView::createPipeline(PipelineManager &pipelineManager) {
    auto & app = getDiligentAppBase();
    rt.create(PIPELINE_KEY, pipelineManager, app.m_pSwapChain, app.m_pDevice);
}

void ImGuiView::switchToPipeline(PipelineManager &pipelineManager) {

}

void ImGuiView::bindShaderResources(PipelineManager &pipelineManager) {

}

void ImGuiView::destroyPipeline(PipelineManager &pipelineManager) {
    rt.destroy(pipelineManager);
}
