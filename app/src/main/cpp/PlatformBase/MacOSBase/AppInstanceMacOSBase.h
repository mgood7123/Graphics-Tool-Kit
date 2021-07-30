//
// Created by Matthew Good on 15/7/21.
//

#ifndef GRAPHICAL_TOOL_KIT_APPINSTANCEMACOSBASE_H
#define GRAPHICAL_TOOL_KIT_APPINSTANCEMACOSBASE_H

class AppInstanceMacOSBase {
public:
    // delete called on non-final that has virtual functions but non-virtual destructor
    virtual ~AppInstanceMacOSBase() = default;
    virtual void swapBuffers();
};

#endif //GRAPHICAL_TOOL_KIT_APPINSTANCEMACOSBASE_H
