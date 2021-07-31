//
// Created by Matthew Good on 30/6/21.
//

#ifndef GRAPHICAL_TOOL_KIT_RECTANGLEVIEW_H
#define GRAPHICAL_TOOL_KIT_RECTANGLEVIEW_H

#include "View.h"

class RectangleView : public View
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
};


#endif //GRAPHICAL_TOOL_KIT_RECTANGLE_H
