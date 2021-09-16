//
// Created by Matthew Good on 8/8/21.
//

#ifndef GRAPHICAL_TOOL_KIT_GRIDVIEW_H
#define GRAPHICAL_TOOL_KIT_GRIDVIEW_H

#include "ViewGroup.h"

class GridView : public ViewGroup {
public:
    const char *getPipelineKeyA() override;

    const char *getPipelineKeyB() override;

    virtual void onMeasure() override;
    virtual void onLayout(bool changed, const Rectangle &dimensions, DrawTools &drawTools, RenderTarget &screenRenderTarget, RenderTarget &renderTarget) override;
};


#endif //GRAPHICAL_TOOL_KIT_GRIDVIEW_H
