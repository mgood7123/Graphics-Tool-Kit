//
// Created by Matthew Good on 8/8/21.
//

#include "GridView.h"

void GridView::onCreatePipeline(PipelineManager &pipelineManager) {
    auto & app = getDiligentAppBase();
    rt.create(PIPELINE_KEY, pipelineManager, app.m_pSwapChain, app.m_pDevice);
    rt2.create(PIPELINE_KEY2, pipelineManager, app.m_pSwapChain, app.m_pDevice);
}

void GridView::onDestroyPipeline(PipelineManager &pipelineManager) {
    rt.destroy(pipelineManager);
    rt2.destroy(pipelineManager);
}

void GridView::onResize(PipelineManager &pipelineManager) {
    auto & app = getDiligentAppBase();
    rt.resize(pipelineManager, app.m_pSwapChain, app.m_pDevice);
    rt2.resize(pipelineManager, app.m_pSwapChain, app.m_pDevice);
}

void GridView::onMeasure() {
    auto array = getChildren();
    for (View *view : array) view->measure();
    setMeasuredDimensions(100, 100);
}

void GridView::onLayout(bool changed, const Rectangle &dimensions, DrawTools &drawTools, RenderTarget &screenRenderTarget, RenderTarget &renderTarget) {
    
    auto canvasSize = getVirtualCanvasSize();
    Position half = canvasSize / 2;

    auto array = getChildren();
    size_t count = 0;
    for (View *view : array) {
        Rectangle drawPosition;

        if (view->offsetInVirtualCanvas < half) {
            LOOP:
            switch (count) {
                case 0:
                    drawPosition = {0, 0, half.x, half.y};
                    break;
                case 1:
                    drawPosition = {half.x, 0, canvasSize.x, half.y};
                    break;
                case 2:
                    drawPosition = {0, half.y, half.x, canvasSize.y};
                    break;
                case 3:
                    drawPosition = {half.x, half.y, canvasSize};
                    break;
                default:
                    // wrap around
                    count = 0;
                    goto LOOP;
            }
            drawTools.pixelToNDC.resize(canvasSize.x, canvasSize.y);
            view->buildCoordinates(drawPosition, drawTools, renderTarget);
            view->layout(view->getRelativePosition(), drawTools, screenRenderTarget, renderTarget);
        }
        count++;
    }
}

void GridView::draw(DrawTools & drawTools, RenderTarget & screenRenderTarget, RenderTarget & renderTarget) {
    auto & app = getDiligentAppBase();

    rt.bind(app.m_pImmediateContext);
    rt.clearColorAndDepth(RenderTarget::black, 1.0f, app.m_pImmediateContext);

    auto canvasSize = getVirtualCanvasSize();
    
    auto array = getChildren();
    for (View *view : array) {
        // if obj == this
        //     draw render target to temporary render target
        //     and then draw temporary render target to render target
        //     this avoids artifacts that might occur
        //     when the render target is drawing to itself
        // otherwise
        //     draw the object to rt and then draw rt to render target

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
    
    rt.drawToRenderTarget(drawTools, renderTarget, app.m_pImmediateContext);
}
