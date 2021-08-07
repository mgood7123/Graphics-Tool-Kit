//
// Created by Matthew Good on 30/6/21.
//

#ifndef GRAPHICAL_TOOL_KIT_OBJECTBASE_H
#define GRAPHICAL_TOOL_KIT_OBJECTBASE_H

#include <DiligentHelpers/DiligentAppBase.h>
#include <VertexEngine/VertexEngine.h>
#include <MultiTouch/MultiTouch.h>

class ObjectBase
{
public:
    DiligentAppBase * diligentAppBase = nullptr;

    virtual void createPipeline(PipelineManager & pipelineManager);
    virtual void switchToPipeline(PipelineManager & pipelineManager);
    virtual void bindShaderResources(PipelineManager & pipelineManager);
    virtual void destroyPipeline(PipelineManager & pipelineManager);
    virtual void create();
    virtual void resize(PipelineManager & pipelineManager);
    virtual void draw(PipelineManager & pipelineManager);
    virtual bool onTouchEvent(MultiTouch &touch);
    virtual void destroy();
    virtual bool hasPhysics();
    virtual void physics(const TimeEngine & timeEngine);
    // delete called on non-final that has virtual functions but non-virtual destructor
    virtual ~ObjectBase() = default;

    static double degreesToRadians(double y);
};


#endif //GRAPHICAL_TOOL_KIT_OBJECTBASE_H
