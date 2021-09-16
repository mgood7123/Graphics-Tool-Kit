//
// Created by Matthew Good on 8/8/21.
//

#include "GridView.h"

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

const char *GridView::getPipelineKeyA() {
    return "GridView A";
}

const char *GridView::getPipelineKeyB() {
    return "GridView B";
}
