//
// Created by Matthew Good on 4/9/21.
//

#ifndef GRAPHICAL_TOOL_KIT_LINEARLAYOUT_H
#define GRAPHICAL_TOOL_KIT_LINEARLAYOUT_H

#include "ViewGroup.h"

class LinearLayout : public ViewGroup {
public:
    enum Orientation {
        Horizontal, Vertical
    };
    
private:
    RenderTarget rt;
    RenderTarget rt2;
    static constexpr const char * PIPELINE_KEY = "GridView ORT";
    static constexpr const char * PIPELINE_KEY2 = "GridView ORT2";
    Orientation orientation = Vertical;
public:
    void setOrientation(Orientation orientation);
    Orientation getOrientation();
    void onCreatePipeline(PipelineManager & pipelineManager) override;
    void onDestroyPipeline(PipelineManager & pipelineManager) override;
    void onResize(PipelineManager & pipelineManager) override;
    void draw(DrawTools &drawTools, RenderTarget &screenRenderTarget, RenderTarget &renderTarget) override;

    virtual void onMeasure() override;
};

#endif //GRAPHICAL_TOOL_KIT_LINEARLAYOUT_H
