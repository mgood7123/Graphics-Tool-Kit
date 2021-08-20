//
// Created by Matthew Good on 30/6/21.
//

#ifndef GRAPHICAL_TOOL_KIT_TOUCH_DETECTORPAINTER_H
#define GRAPHICAL_TOOL_KIT_TOUCH_DETECTORPAINTER_H

#include "PainterView.h"

class TouchDetectorPainter : public PainterView
{
    VertexEngine::Canvas::Color4 color {1,0,0,1};
public:

    void onDraw(VertexEngine::Canvas &canvas) override;
    bool onTouchEvent(MultiTouch &touch) override;

    int getCanvasWidth() override;

    int getCanvasHeight() override;
};


#endif //GRAPHICAL_TOOL_KIT_TOUCH_DETECTORPAINTER_H
