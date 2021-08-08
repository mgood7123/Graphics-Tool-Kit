//
// Created by Matthew Good on 8/8/21.
//

#ifndef GRAPHICAL_TOOL_KIT_CUBEONTRIANGLE_H
#define GRAPHICAL_TOOL_KIT_CUBEONTRIANGLE_H

#include "ObjectBase.h"
#include "Cube.h"
#include "Triangle.h"

class CubeOnTriangle : public ObjectBase {
    Cube cube;
    Triangle triangle;
    RenderTarget rt;
    static constexpr const char * PIPELINE_KEY = "CubeOnTriangle ORT";
public:
    void setDiligentAppBase(DiligentAppBase *diligentAppBase) override;

    void createPipeline(PipelineManager & pipelineManager) override;
    void destroyPipeline(PipelineManager & pipelineManager) override;
    void create() override;
    void resize(PipelineManager & pipelineManager) override;
    void draw(DrawTools & drawTools, RenderTarget & renderTarget) override;
    bool onTouchEvent(MultiTouch &touch) override;
    void destroy() override;
    bool hasPhysics() override;
    void physics(const TimeEngine & timeEngine) override;
};


#endif //GRAPHICAL_TOOL_KIT_CUBEONTRIANGLE_H
