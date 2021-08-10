//
// Created by Matthew Good on 8/8/21.
//

#ifndef GRAPHICAL_TOOL_KIT_CUBEONTRIANGLE_H
#define GRAPHICAL_TOOL_KIT_CUBEONTRIANGLE_H

#include "ObjectGroup.h"
#include "Cube.h"
#include "Triangle.h"
#include "Painters/RectanglePainter.h"

class CubeOnTriangle : public ObjectGroup {
    RenderTarget rt;
    static constexpr const char * PIPELINE_KEY = "CubeOnTriangle ORT";
public:
    void createPipeline(PipelineManager & pipelineManager) override;
    void destroyPipeline(PipelineManager & pipelineManager) override;
    void create() override;
    void resize(PipelineManager & pipelineManager) override;
    void draw(DrawTools & drawTools, RenderTarget & renderTarget) override;
    bool onTouchEvent(MultiTouch &touch) override;
    void destroy() override;
};


#endif //GRAPHICAL_TOOL_KIT_CUBEONTRIANGLE_H
