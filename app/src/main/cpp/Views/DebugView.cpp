//
// Created by Matthew Good on 4/9/21.
//

#include "DebugView.h"

void DebugView::draw(DrawTools &drawTools, RenderTarget &screenRenderTarget, RenderTarget &renderTarget) {

    // IMPORTANT: clear render target before doing anything else

    auto & app = getDiligentAppBase();
    rt.bind(app.m_pImmediateContext);
    rt.clearColorAndDepth(RenderTarget::black, 1.0f, app.m_pImmediateContext);

    // we only support a single child

    View * view = getChildAt(0);


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

void DebugView::onCreatePipeline(PipelineManager &pipelineManager) {
    auto & app = getDiligentAppBase();
    rt.create(PIPELINE_KEY, pipelineManager, app.m_pSwapChain, app.m_pDevice);
}

void DebugView::onDestroyPipeline(PipelineManager &pipelineManager) {
    rt.destroy(pipelineManager);
}

void DebugView::onResize(PipelineManager &pipelineManager) {
    auto & app = getDiligentAppBase();
    rt.resize(pipelineManager, app.m_pSwapChain, app.m_pDevice);
}

void DebugView::addView(View * view) {
    if (view == this) {
        Log::Error_And_Throw("DebugView does not support adding itself as a child");
    }
    if (getChildCount() == 1) {
        Log::Error_And_Throw("DebugView only supports a single child");
    }
    ViewGroup::addView(view);
}
