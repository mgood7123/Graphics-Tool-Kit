//
// Created by Matthew Good on 8/8/21.
//

#ifndef GRAPHICAL_TOOL_KIT_GRIDVIEW_H
#define GRAPHICAL_TOOL_KIT_GRIDVIEW_H

#include "ViewGroup.h"

class GridView : public ViewGroup {
    RenderTarget rt;
    RenderTarget rt2;
    static constexpr const char * PIPELINE_KEY = "GridView ORT";
    static constexpr const char * PIPELINE_KEY2 = "GridView ORT2";
public:
    void onCreatePipeline(PipelineManager & pipelineManager) override;
    void onDestroyPipeline(PipelineManager & pipelineManager) override;
    void onCreate() override;
    void onResize(PipelineManager & pipelineManager) override;
    void draw(DrawTools &drawTools, RenderTarget &screenRenderTarget, RenderTarget &renderTarget) override;
    void drawRenderTarget(DrawTools &tools, Diligent::IDeviceContext *deviceContext) override;
    bool onTouchEvent(MultiTouch &touch) override;
    void onDestroy() override;
};


#endif //GRAPHICAL_TOOL_KIT_GRIDVIEW_H
