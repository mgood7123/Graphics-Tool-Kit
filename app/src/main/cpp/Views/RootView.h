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
public:
    virtual const char *getPipelineKeyA() override;

    virtual const char *getPipelineKeyB() override;

    virtual void onMeasure() override;
    virtual void onLayout(bool changed, const Rectangle &dimensions, DrawTools &drawTools, RenderTarget &screenRenderTarget, RenderTarget &renderTarget) override;
    virtual void addView(View * view) override;
};

#endif //GRAPHICAL_TOOL_KIT_ROOTVIEW_H
