//
// Created by Matthew Good on 30/6/21.
//

#ifndef GRAPHICAL_TOOL_KIT_RECTANGLEPAINTER_H
#define GRAPHICAL_TOOL_KIT_RECTANGLEPAINTER_H

#include "Painter.h"

class RectanglePainter : public Painter
{
    float x, y;
public:
    VertexEngine::Canvas::Color4 green = {0, 1, 0, 1};
    VertexEngine::Canvas::Color4 blue = {0, 0, 1, 1};
    VertexEngine::Canvas::Color4 pink = {1, 0, 1, 1};
    VertexEngine::Canvas::Color4 red = {1, 0, 0, 1};

    void onCreate(VertexEngine::TextureManager & textureManager) override;

    bool onTouchEvent(MultiTouch &touch) override;

    void onDraw(VertexEngine::Canvas &canvas) override;
    void onDestroy(VertexEngine::TextureManager & textureManager) override;

    int getCanvasWidth() override;

    int getCanvasHeight() override;
};


#endif //GRAPHICAL_TOOL_KIT_RECTANGLE_H
