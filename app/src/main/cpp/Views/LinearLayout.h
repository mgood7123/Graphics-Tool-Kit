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
    Orientation orientation = Vertical;
public:
    virtual const char *getPipelineKeyA() override;

    virtual const char *getPipelineKeyB() override;

    void setOrientation(Orientation orientation);
    Orientation getOrientation();
    virtual void onMeasure() override;
    virtual void onLayout(bool changed, const Rectangle &dimensions, DrawTools &drawTools, RenderTarget &screenRenderTarget, RenderTarget &renderTarget) override;
    virtual void addView(View *view) override;
    virtual void addView(View *view, float weight);
};

#endif //GRAPHICAL_TOOL_KIT_LINEARLAYOUT_H
