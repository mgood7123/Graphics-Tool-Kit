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
    // for a linear layout, we divide the size by the children count

    float count = getChildCount();

    float view_full_width = 600;
    float view_full_height = 600;

    float view_width = view_full_width;
    float view_height = view_full_height;

    if (orientation == Vertical) {
        view_height /= count;
    } else {
        view_width /= count;
    }

    Rectangle drawPosition = {0, 0, view_width, view_height};

    for (size_t i = 0; i < count; i++) {
//        measurementData.add(drawPosition);
        if (orientation == Vertical) {
            drawPosition.topLeft.y += view_height;
        } else {
            drawPosition.topLeft.x += view_width;
        }
    }
}

void LinearLayout::draw(DrawTools & drawTools, RenderTarget & screenRenderTarget, RenderTarget & renderTarget) {
    auto & app = getDiligentAppBase();

    rt.bind(app.m_pImmediateContext);
    rt.clearColorAndDepth(RenderTarget::black, 1.0f, app.m_pImmediateContext);

    float view_full_width = 600;
    float view_full_height = 600;

    float count = getChildCount();

    for (size_t i = 0; i < count; i++) {
        View * view = getChildAt(i);
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
            // resize the grid to full before building coordinates
            drawTools.pixelToNDC.resize(view_full_width, view_full_height);
//            view->buildCoordinates(measurementData[i], drawTools, renderTarget);
            view->draw(drawTools, screenRenderTarget, rt2);
        }

        // children have likely resized the pixel grid, restore it
        drawTools.pixelToNDC.resize(view_full_width, view_full_height);
        
        if (view == this) {
            rt2.bind(app.m_pImmediateContext);
            rt2.clearColorAndDepth(RenderTarget::black, 1.0f, app.m_pImmediateContext);
//            renderTarget.drawAbsolutePosition(drawTools, measurementData[i], app.m_pImmediateContext);
        }

        rt.bind(app.m_pImmediateContext);
        rt2.drawAbsolutePositionAndClipToBoundaries(
            drawTools, view->getAbsolutePosition(), app.m_pImmediateContext
        );
    }
    
    rt.drawToRenderTarget(drawTools, renderTarget, app.m_pImmediateContext);
}
