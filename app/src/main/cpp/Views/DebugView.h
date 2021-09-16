//
// Created by Matthew Good on 4/9/21.
//

#ifndef GRAPHICAL_TOOL_KIT_DEBUGVIEW_H
#define GRAPHICAL_TOOL_KIT_DEBUGVIEW_H

#include "ViewGroup.h"
#include "LinearLayout.h"

class DebugView : public ViewGroup {
    LinearLayout * linearLayout = nullptr;
    View * contentView = nullptr;
public:
    virtual const char *getPipelineKeyA() override;
    virtual const char *getPipelineKeyB() override;
    virtual void onCreate() override;
    virtual void addView(View * view) override;

    virtual void onDestroy() override;
};

#endif //GRAPHICAL_TOOL_KIT_DEBUGVIEW_H
