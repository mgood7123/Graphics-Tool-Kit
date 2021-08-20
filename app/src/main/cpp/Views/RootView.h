//
// Created by Matthew Good on 14/8/21.
//

#ifndef GRAPHICAL_TOOL_KIT_ADOPTVIEW_H
#define GRAPHICAL_TOOL_KIT_ADOPTVIEW_H

#include "ViewGroup.h"

// rootView cannot be inherited from
class RootView final : public ViewGroup {
    RenderTarget rt;
    static constexpr const char * PIPELINE_KEY = "RootView RT";
public:

    void draw(DrawTools &drawTools, RenderTarget &screenRenderTarget, RenderTarget &renderTarget) override;

    void drawRenderTarget(DrawTools &tools, Diligent::IDeviceContext *deviceContext) override;

    virtual void onCreatePipeline(PipelineManager &pipelineManager) override;

    virtual void onDestroyPipeline(PipelineManager &pipelineManager) override;

    virtual void onResize(PipelineManager &pipelineManager) override;
    
    virtual void addView(View * view) override;
};

#endif //GRAPHICAL_TOOL_KIT_ADOPTVIEW_H
