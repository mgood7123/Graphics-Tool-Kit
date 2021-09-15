//
// Created by Matthew Good on 14/8/21.
//

#ifndef GRAPHICAL_TOOL_KIT_ROOTVIEW_H
#define GRAPHICAL_TOOL_KIT_ROOTVIEW_H

#include "ViewGroup.h"

/** the simplest form of a ViewGroup <br>
 * a RootView supports only a single child and acts like a container,
 * placing its child within itself at full scale <br>
 * a RootView supports padding
 */
class RootView : public ViewGroup {
    RenderTarget rt;
    RenderTarget rt2;
    static constexpr const char * PIPELINE_KEY = "RootView RT";
    static constexpr const char * PIPELINE_KEY2 = "RootView RT2";
public:

    virtual void onMeasure() override;
    virtual void onLayout(bool changed, const Rectangle &dimensions, DrawTools &drawTools, RenderTarget &screenRenderTarget, RenderTarget &renderTarget) override;

    virtual void draw(DrawTools &drawTools, RenderTarget &screenRenderTarget, RenderTarget &renderTarget) override;

    virtual void onCreatePipeline(PipelineManager &pipelineManager) override;

    virtual void onDestroyPipeline(PipelineManager &pipelineManager) override;

    virtual void onResize(PipelineManager &pipelineManager) override;
    
    virtual void addView(View * view) override;
};

#endif //GRAPHICAL_TOOL_KIT_ROOTVIEW_H
