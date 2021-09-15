//
// Created by Matthew Good on 4/9/21.
//

#include "LinearLayout.h"

// in order to implement layout weight, or wrap_content
// we must implement a multi pass render: measure, layout, draw
//
// 23:45:23 Roughy: measure/layout should definitely only be done /once/ and then as needed.
// The same goes for drawing; only invalidate the view if it actually needs to be updated.


void LinearLayout::setOrientation(LinearLayout::Orientation orientation) {
    this->orientation = orientation;
}

LinearLayout::Orientation LinearLayout::getOrientation() {
    return orientation;
}

void LinearLayout::onCreatePipeline(PipelineManager &pipelineManager) {
    auto & app = getDiligentAppBase();
    rt.create(PIPELINE_KEY, pipelineManager, app.m_pSwapChain, app.m_pDevice);
    rt2.create(PIPELINE_KEY2, pipelineManager, app.m_pSwapChain, app.m_pDevice);
}

void LinearLayout::onDestroyPipeline(PipelineManager &pipelineManager) {
    rt.destroy(pipelineManager);
    rt2.destroy(pipelineManager);
}

void LinearLayout::onResize(PipelineManager &pipelineManager) {
    auto & app = getDiligentAppBase();
    rt.resize(pipelineManager, app.m_pSwapChain, app.m_pDevice);
    rt2.resize(pipelineManager, app.m_pSwapChain, app.m_pDevice);
}

void LinearLayout::onMeasure() {
    auto array = getChildren();
    for (View *view : array) view->measure();
    setMeasuredDimensions(100, 100);
}

void LinearLayout::onLayout(bool changed, const Rectangle &dimensions, DrawTools &drawTools, RenderTarget &screenRenderTarget, RenderTarget &renderTarget) {
    // for a linear layout, we divide the size by the children count

    auto canvasSize = getVirtualCanvasSize();
    Position section = canvasSize / (float) getChildCount();

    auto array = getChildren();
    size_t count = 0;
    for (View *view : array) {
        Rectangle drawPosition;
        if (orientation == Horizontal) {
            drawPosition = {section.x * count, 0, section.x * (count + 1), canvasSize.y};
        } else {
            drawPosition = {0, section.y * count, canvasSize.x, section.y * (count + 1)};
        }
        drawTools.pixelToNDC.resize(canvasSize.x, canvasSize.y);
        view->buildCoordinates(drawPosition, drawTools, renderTarget);
        view->layout(view->getRelativePosition(), drawTools, screenRenderTarget, renderTarget);
        count++;
    }
}

void LinearLayout::draw(DrawTools & drawTools, RenderTarget & screenRenderTarget, RenderTarget & renderTarget) {
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
