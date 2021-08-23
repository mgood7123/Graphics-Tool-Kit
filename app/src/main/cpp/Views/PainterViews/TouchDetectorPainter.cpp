//
// Created by Matthew Good on 30/6/21.
//

#include "TouchDetectorPainter.h"

int TouchDetectorPainter::getCanvasWidth() {
    return 1;
}

int TouchDetectorPainter::getCanvasHeight() {
    return 1;
}

void TouchDetectorPainter::onDraw(VertexEngine::Canvas &canvas) {
    canvas.fill(color);
}

bool TouchDetectorPainter::onTouchEvent(MultiTouch &touch) {
    auto t = touch.getTouchAt(touch.getTouchIndex());
    if (t.state == MultiTouch::TOUCH_DOWN) {
        color = {0,1,0,1};
    } else if (t.state == MultiTouch::TOUCH_UP || t.state == MultiTouch::TOUCH_CANCELLED) {
        color = {1,0,0,1};
    }
    return true;
}
