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

    //23:16:40 Roughy: layout_weight is basically just the normalized weight * view size, except the weights kinda work in reverse so if you have 1.0 and 2.0 the 1.0 will take up 2/3rds'ish instead of the more logical opposite, if I Recall correctly
    //23:16:58 smallville7123: this is what i have at the moment
    //23:21:26 Roughy: if the weights made sense you'd just do ( weight / sum_of_weights ) * total_size_to_fill.
    //23:23:03 Roughy: in the case of Android's reversed weight values ( 1 - (weight / sum_of_weights ) ) I guess
    //23:26:01 smallville7123: so for example, if two views take up 50 each, and parent is 100, then
    //23:26:50 smallville7123: child_1.section = ( 1 - (childAt(0).weight / sum_of_weights ) );      child_2.section = ( 1 - (childAt(1).weight / sum_of_weights ) )
    //23:27:08 smallville7123: _section  *
    //23:28:47 Roughy: that should give you 0.5 for each of them. Multiply by 100 gives you 50 for each

    auto canvasSize = getVirtualCanvasSize();

    auto array = getChildren();

    float weightSum = 0;

    for(View *view : array) weightSum += view->weight;

    bool first = true;
    Rectangle drawPosition = 0;
    float section = 0;
    float prevSection = 0;
    for (View *view : array) {
        float multiplier = (view->weight / weightSum);
        prevSection = section;
        if (orientation == Horizontal) {
            section = multiplier * canvasSize.x;
            if (first) {
                first = false;
                drawPosition.bottomRight.y = canvasSize.y;
            } else {
                drawPosition.topLeft.x += prevSection;
            }
            drawPosition.bottomRight.x += section;
        } else {
            section = multiplier * canvasSize.y;
            if (first) {
                first = false;
                drawPosition.bottomRight.x = canvasSize.x;
            } else {
                drawPosition.topLeft.y += prevSection;
            }
            drawPosition.bottomRight.y += section;
        }
        drawTools.pixelToNDC.resize(canvasSize.x, canvasSize.y);
        view->buildCoordinates(drawPosition, drawTools, renderTarget);
        view->layout(view->getRelativePosition(), drawTools, screenRenderTarget, renderTarget);
    }
}

void LinearLayout::addView(View *view, float weight) {
    view->weight = weight;
    ViewGroup::addView(view);
}

void LinearLayout::addView(View *view) {
    view->weight = 1;
    ViewGroup::addView(view);
}

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
