//
// Created by Matthew Good on 30/6/21.
//

#include "ColorPainter.h"

int ColorPainter::getCanvasWidth() {
    return 1;
}

int ColorPainter::getCanvasHeight() {
    return 1;
}

void ColorPainter::onDraw(VertexEngine::Canvas &canvas) {
    canvas.fill(color);
}