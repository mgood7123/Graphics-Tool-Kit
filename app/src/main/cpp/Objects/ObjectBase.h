//
// Created by Matthew Good on 30/6/21.
//

#ifndef GRAPHICAL_TOOL_KIT_OBJECTBASE_H
#define GRAPHICAL_TOOL_KIT_OBJECTBASE_H

#include <DiligentAppBase.h>
#include <VertexEngine.h>

class ObjectBase
{
public:
    DiligentAppBase * diligentAppBase = nullptr;
    VertexEngine createVertexEngine(int width, int height);

    virtual void create();
    virtual void draw();
    virtual void destroy();
    virtual void physics(const TimeEngine & timeEngine);
    // delete called on non-final that has virtual functions but non-virtual destructor
    virtual ~ObjectBase() = default;

    static double degreesToRadians(double y);
};


#endif //GRAPHICAL_TOOL_KIT_OBJECTBASE_H
