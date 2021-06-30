//
// Created by Matthew Good on 30/6/21.
//

#ifndef GRAPHICAL_TOOL_KIT_TRIANGLE_H
#define GRAPHICAL_TOOL_KIT_TRIANGLE_H


#include "ObjectBase.h"

class Triangle : public ObjectBase
{
public:
    static const char * triangle_VS;
    static const char * triangle_PS;
    void create(DiligentAppBase * base) override;
    void draw(DiligentAppBase * base) override;
};


#endif //GRAPHICAL_TOOL_KIT_TRIANGLE_H
