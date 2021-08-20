//
// Created by Matthew Good on 30/6/21.
//

#ifndef GRAPHICAL_TOOL_KIT_TRIANGLE_H
#define GRAPHICAL_TOOL_KIT_TRIANGLE_H

#include "View.h"

class TriangleView : public View
{
public:
    static const char * triangle_VS;
    static const char * triangle_PS;
    static constexpr const char * PIPELINE_KEY = "TriangleView";

    void createPipeline(PipelineManager & pipelineManager) override;
    void switchToPipeline(PipelineManager & pipelineManager) override;
    void bindShaderResources(PipelineManager & pipelineManager) override;
    void destroyPipeline(PipelineManager & pipelineManager) override;

    void draw(DrawTools &drawTools, RenderTarget &screenRenderTarget, RenderTarget &renderTarget) override;
};


#endif //GRAPHICAL_TOOL_KIT_TRIANGLE_H
