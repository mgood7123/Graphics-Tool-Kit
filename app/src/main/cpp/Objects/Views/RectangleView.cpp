//
// Created by Matthew Good on 30/6/21.
//

#include "RectangleView.h"

void RectangleView::onDraw(VertexEngine::Canvas &canvas) {
    canvas.clear();

    // from [x, y] to [x, y]
    canvas.planeAt(20, 20, 200, 200, green);
    // from [x, y] to [x+width, y+width]
    canvas.plane(140, 140, 200, 200, blue);
    canvas.plane(160, 160, 80, 80, pink);

    VertexEngine::Canvas subcanvas = canvas.subCanvas(200, 200, 30, 30);
    subcanvas.clear({1,0,0,1});
}
