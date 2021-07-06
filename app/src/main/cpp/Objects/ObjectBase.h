//
// Created by Matthew Good on 30/6/21.
//

#ifndef GRAPHICAL_TOOL_KIT_OBJECTBASE_H
#define GRAPHICAL_TOOL_KIT_OBJECTBASE_H


#include <DiligentAppBase.h>

class ObjectBase
{
public:
    DiligentAppBase * diligentAppBase;
    virtual void create();
    virtual void draw();
    // delete called on non-final that has virtual functions but non-virtual destructor
    virtual ~ObjectBase() = default;
};


#endif //GRAPHICAL_TOOL_KIT_OBJECTBASE_H
