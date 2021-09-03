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
    onCreate();
}

void ImGuiView::resize(PipelineManager &pipelineManager) {
    ImGui::GetIO().DisplaySize = {
            static_cast<float>(getDiligentAppBase().swapChainWidth),
            static_cast<float>(getDiligentAppBase().swapChainHeight)
    };
}

void ImGuiView::generateFontTextures() {
    m_pImGui->UpdateFontsTexture();
}

void
ImGuiView::draw(DrawTools &drawTools, RenderTarget &screenRenderTarget, RenderTarget &renderTarget) {

    auto & app = getDiligentAppBase();

    renderTarget.bind(app.m_pImmediateContext);
    renderTarget.clearColorAndDepth(
            RenderTarget::black, 1.0f, app.m_pImmediateContext
    );

    beforeFrame();

    m_pImGui->NewFrame(
            app.swapChainWidth,
            app.swapChainHeight,
            app.m_pSwapChain->GetDesc().PreTransform
    );

    onDraw();

    m_pImGui->Render(app.m_pImmediateContext);
}

void ImGuiView::destroy() {
    onDestroy();
    m_pImGui->InvalidateDeviceObjects();
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

}

void ImGuiView::switchToPipeline(PipelineManager &pipelineManager) {

}

void ImGuiView::bindShaderResources(PipelineManager &pipelineManager) {

}

void ImGuiView::destroyPipeline(PipelineManager &pipelineManager) {

}
