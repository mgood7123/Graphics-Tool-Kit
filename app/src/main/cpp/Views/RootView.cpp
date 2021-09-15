//
// Created by Matthew Good on 14/8/21.
//

#include "RootView.h"

void RootView::onMeasure() {
    View * view = getChildAt(0);
    if (view != nullptr) view->measure();
    setMeasuredDimensions(100, 100);
}

void RootView::onLayout(bool changed, const Rectangle &dimensions, DrawTools &drawTools, RenderTarget &screenRenderTarget, RenderTarget &renderTarget) {
    View * view = getChildAt(0);
    if (view != nullptr) {
        auto dim = view->getMeasuredDimensions();
        
        auto canvasSize = getVirtualCanvasSize();

        float w = minmax(dim.x, canvasSize.x);
        float h = minmax(dim.y, canvasSize.y);

        drawTools.pixelToNDC.resize(canvasSize.x, canvasSize.y);
        view->buildCoordinates({0, 0, w, h}, drawTools, renderTarget);
        view->layout(view->getRelativePosition(), drawTools, screenRenderTarget, renderTarget);
    }
}

void RootView::onResize(PipelineManager &pipelineManager) {
    auto & app = getDiligentAppBase();
    rt.resize(pipelineManager, app.m_pSwapChain, app.m_pDevice);
    rt2.resize(pipelineManager, app.m_pSwapChain, app.m_pDevice);
}

void RootView::draw(DrawTools &drawTools, RenderTarget &screenRenderTarget, RenderTarget &renderTarget) {

    // IMPORTANT: clear render target before doing anything else

    auto & app = getDiligentAppBase();
    rt.bind(app.m_pImmediateContext);
    rt.clearColorAndDepth({1,0,1,1}, 1.0f, app.m_pImmediateContext);

    auto canvasSize = getVirtualCanvasSize();
    
    // we only support a single child

    View * view = getChildAt(0);

    if (view != nullptr) {
        // we draw fill entire contents

        // draw object to render target `rt`
        rt2.bind(app.m_pImmediateContext);
        rt2.clearColorAndDepth(RenderTarget::black, 1.0f, app.m_pImmediateContext);
        view->switchToPipeline(drawTools.pipelineManager);
        view->bindShaderResources(drawTools.pipelineManager);
        // children have likely resized the pixel grid, restore it
        drawTools.pixelToNDC.resize(canvasSize.x, canvasSize.y);
        view->draw(drawTools, screenRenderTarget, rt2);

        rt.bind(app.m_pImmediateContext);
        
        Position drawDimensions = view->getDrawDimensions();
        Rectangle drawPosition = view->getDrawPosition();

        if (printLogging) {
            Log::Info("TAG: ", getTag(), " -> ", view->getTag(), ", resizing pixel grid to             : ", drawDimensions);
        }
        drawTools.pixelToNDC.resize(drawDimensions.x, drawDimensions.y);
        rt2.clip(app.m_pImmediateContext);
        if (printLogging) {
            Log::Info("TAG: ", getTag(), " -> ", view->getTag(), ", drawing absolute position          : ", drawPosition);
        }
        rt2.drawAbsolutePosition(drawTools, drawPosition, app.m_pImmediateContext);
    }
    
    // drawToRenderTarget modifies pixel grid
    rt.drawToRenderTarget(drawTools, renderTarget, app.m_pImmediateContext);
}

void RootView::onCreatePipeline(PipelineManager &pipelineManager) {
    auto & app = getDiligentAppBase();
    rt.create(PIPELINE_KEY, pipelineManager, app.m_pSwapChain, app.m_pDevice);
    rt2.create(PIPELINE_KEY2, pipelineManager, app.m_pSwapChain, app.m_pDevice);
}

void RootView::onDestroyPipeline(PipelineManager &pipelineManager) {
    rt.destroy(pipelineManager);
    rt2.destroy(pipelineManager);
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
