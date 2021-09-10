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

        float view_full_width = 100;
        float view_full_height = 100;

        float w = minmax(dim.x, view_full_width);
        float h = minmax(dim.y, view_full_height);

        drawTools.pixelToNDC.resize(view_full_width, view_full_height);
        view->buildCoordinates({0, 0, w, h}, drawTools, renderTarget);
        view->layout(view->getRelativePosition(), drawTools, screenRenderTarget, renderTarget);
        // TODO: do the coordinates need to be rebuilt after layout?
    }
}

void RootView::draw(DrawTools &drawTools, RenderTarget &screenRenderTarget, RenderTarget &renderTarget) {

    // IMPORTANT: clear render target before doing anything else

    auto & app = getDiligentAppBase();
    rt.bind(app.m_pImmediateContext);
    rt.clearColorAndDepth({1,0,1,1}, 1.0f, app.m_pImmediateContext);

    float view_full_width = 100;
    float view_full_height = 100;

    // we only support a single child

    View * view = getChildAt(0);

    if (view != nullptr) {
        // we draw fill entire contents

        // if obj == this
        //     draw render target to temporary render target
        //     and then draw temporary render target to render target
        //     this avoids artifacts that might occur
        //     when the render target is drawing to itself
        // otherwise
        //     draw the object to rt and then draw rt to render target

        if (view != this) {
            // draw object to render target `rt`
            rt2.bind(app.m_pImmediateContext);
            rt2.clearColorAndDepth(RenderTarget::black, 1.0f, app.m_pImmediateContext);
            view->switchToPipeline(drawTools.pipelineManager);
            view->bindShaderResources(drawTools.pipelineManager);
            // children have likely resized the pixel grid, restore it
            drawTools.pixelToNDC.resize(view_full_width, view_full_height);
            view->draw(drawTools, screenRenderTarget, rt2);
        }

        // children have likely resized the pixel grid, restore it
        drawTools.pixelToNDC.resize(view_full_width, view_full_height);

        rt.bind(app.m_pImmediateContext);
        rt2.drawAbsolutePositionAndClipToBoundaries(
            drawTools, view->getAbsolutePosition(), app.m_pImmediateContext
        );
    }
    
    // children have likely resized the pixel grid, restore it
    drawTools.pixelToNDC.resize(view_full_width, view_full_height);
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

void RootView::onResize(PipelineManager &pipelineManager) {
    auto & app = getDiligentAppBase();
    rt.resize(pipelineManager, app.m_pSwapChain, app.m_pDevice);
    rt2.resize(pipelineManager, app.m_pSwapChain, app.m_pDevice);
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
