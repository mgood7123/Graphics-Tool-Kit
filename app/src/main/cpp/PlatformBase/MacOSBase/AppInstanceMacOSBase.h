//
// Created by Matthew Good on 15/7/21.
//

#ifndef GRAPHICAL_TOOL_KIT_APPINSTANCEMACOSBASE_H
#define GRAPHICAL_TOOL_KIT_APPINSTANCEMACOSBASE_H

#include <SDL.h>

class AppInstanceMacOSBase {
public:
    SDL_Window *mainwindow; /* Our window handle */

    // delete called on non-final that has virtual functions but non-virtual destructor
    virtual ~AppInstanceMacOSBase() = default;
    virtual void swapBuffers();

};


#endif //GRAPHICAL_TOOL_KIT_APPINSTANCEMACOSBASE_H
