//
// Created by Matthew Good on 30/6/21.
//

#ifndef GRAPHICAL_TOOL_KIT_COLORPAINTER_H
#define GRAPHICAL_TOOL_KIT_COLORPAINTER_H

#include "PainterView.h"

class ColorPainter : public PainterView
{
public:
    VertexEngine::Canvas::Color4 color {0,0,0,1};

    void onDraw(VertexEngine::Canvas &canvas) override;

    int getCanvasWidth() override;

    int getCanvasHeight() override;
};


#endif //GRAPHICAL_TOOL_KIT_COLORPAINTER_H
