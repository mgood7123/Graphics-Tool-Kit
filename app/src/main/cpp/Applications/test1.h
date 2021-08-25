//
// Created by Matthew Good on 25/8/21.
//

#ifndef GRAPHICAL_TOOL_KIT_TEST1_H
#define GRAPHICAL_TOOL_KIT_TEST1_H

#include <Application/Application.h>

class test1 : public Application {
public:
    virtual void onCreateViews();
};

MAKE_APPLICATION_PUBLIC__HEADER(test1);

#endif //GRAPHICAL_TOOL_KIT_TEST1_H
