//
// Created by Matthew Good on 14/8/21.
//

#include "RootView.h"

void RootView::draw(DrawTools &drawTools, RenderTarget &screenRenderTarget, RenderTarget &renderTarget) {

    // IMPORTANT: clear render target before doing anything else

    auto & app = getDiligentAppBase();
    rt.bind(app.m_pImmediateContext);
    rt.clearColorAndDepth(RenderTarget::black, 1.0f, app.m_pImmediateContext);

    // we only support a single child

    View * view = viewAt(0);


    // we may not have a child set

    if (view != nullptr) {

        // we draw fill entire contents
        Rectangle drawPosition = {0, 0, 100, 100};

        drawTools.pixelToNDC.resize(100, 100);

        // draw object to render target `rt`
        view->switchToPipeline(drawTools.pipelineManager);
        view->bindShaderResources(drawTools.pipelineManager);
        view->buildCoordinates(drawPosition, drawTools, renderTarget);
        view->draw(drawTools, screenRenderTarget, rt);
        rt.drawToRenderTarget(drawTools, renderTarget, app.m_pImmediateContext);
    }
}

void RootView::onCreatePipeline(PipelineManager &pipelineManager) {
    auto & app = getDiligentAppBase();
    rt.create(PIPELINE_KEY, pipelineManager, app.m_pSwapChain, app.m_pDevice);
}

void RootView::onDestroyPipeline(PipelineManager &pipelineManager) {
    rt.destroy(pipelineManager);
}

void RootView::onResize(PipelineManager &pipelineManager) {
    auto & app = getDiligentAppBase();
    rt.resize(pipelineManager, app.m_pSwapChain, app.m_pDevice);
}

void RootView::addView(View * view) {
    if (view == this) {
        Log::Error_And_Throw("RootView does not support adding itself as a child");
    }
    if (getChildCount() == 1) {
        Log::Error_And_Throw("RootView only supports a single child");
    }
    ViewGroup::addView(view);
}

void RootView::drawRenderTarget(DrawTools &tools, Diligent::IDeviceContext *deviceContext) {
    // traversing the heirarchy can be expensive
    Rectangle x;
    x = {0, 0, rt.getWidth(), rt.getHeight()};
    rt.drawAbsolutePositionAndClipToBoundaries(tools, x, deviceContext);
}
