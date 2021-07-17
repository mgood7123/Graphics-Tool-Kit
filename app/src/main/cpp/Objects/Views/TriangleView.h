//
// Created by Matthew Good on 30/6/21.
//

#ifndef GRAPHICAL_TOOL_KIT_TRIANGLEVIEW_H
#define GRAPHICAL_TOOL_KIT_TRIANGLEVIEW_H

#include "View.h"

class TriangleView : public View
{
public:
    static const char * triangle_VS;
    static const char * triangle_PS;
    void create() override;
    void draw() override;
};


#endif //GRAPHICAL_TOOL_KIT_TRIANGLE_H
