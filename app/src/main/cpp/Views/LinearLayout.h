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

    class LinearLayoutParams : public LayoutParams {
    public:
        float weight;
        LinearLayoutParams() : weight(1.0f) {}
        LinearLayoutParams(float weight) : weight(weight) {}
        LinearLayoutParams(GRAVITY gravity, float weight) : LayoutParams(gravity), weight(weight) {}
    };

    virtual const char *getPipelineKeyA() override;

    virtual const char *getPipelineKeyB() override;

    void setOrientation(Orientation orientation);
    Orientation getOrientation();
    virtual void onMeasure() override;
    virtual void onLayout(bool changed, const Rectangle &dimensions, DrawTools &drawTools, RenderTarget &screenRenderTarget, RenderTarget &renderTarget) override;
};

#endif //GRAPHICAL_TOOL_KIT_LINEARLAYOUT_H
