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

void RootView::addView(View * view) {
    if (view == this) {
        Log::Error_And_Throw("RootView does not support adding itself as a child");
    }
    if (getChildCount() == 1) {
        Log::Error_And_Throw("RootView only supports a single child");
    }
    ViewGroup::addView(view);
}

const char *RootView::getPipelineKeyA() {
    return "RootView A";
}

const char *RootView::getPipelineKeyB() {
    return "RootView B";
}
