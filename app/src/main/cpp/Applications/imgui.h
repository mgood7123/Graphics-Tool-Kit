//
// Created by Matthew Good on 26/8/21.
//

#ifndef GRAPHICAL_TOOL_KIT_APPLICATION_IMGUI_H
#define GRAPHICAL_TOOL_KIT_APPLICATION_IMGUI_H

#include <Application/Application.h>

class imgui : public Application {
public:
    virtual void onCreateViews();
};

MAKE_APPLICATION_PUBLIC__HEADER(imgui);

#endif //GRAPHICAL_TOOL_KIT_APPLICATION_IMGUI_H
