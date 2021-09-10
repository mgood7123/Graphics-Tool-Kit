//
// Created by Matthew Good on 4/9/21.
//

#ifndef GRAPHICAL_TOOL_KIT_DEBUGVIEW_H
#define GRAPHICAL_TOOL_KIT_DEBUGVIEW_H

#include "ViewGroup.h"

class DebugView : public ViewGroup {
    RenderTarget rt;
    static constexpr const char * PIPELINE_KEY = "DebugView RT";
public:

    void draw(DrawTools &drawTools, RenderTarget &screenRenderTarget, RenderTarget &renderTarget) override;

    virtual void onCreatePipeline(PipelineManager &pipelineManager) override;

    virtual void onDestroyPipeline(PipelineManager &pipelineManager) override;

    virtual void onResize(PipelineManager &pipelineManager) override;

    virtual void addView(View * view) override;
};


#endif //GRAPHICAL_TOOL_KIT_DEBUGVIEW_H
