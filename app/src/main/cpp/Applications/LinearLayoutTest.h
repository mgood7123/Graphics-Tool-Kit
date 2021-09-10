//
// Created by Matthew Good on 25/8/21.
//

#ifndef GRAPHICAL_TOOL_KIT_APPLICATION_LINEARLAYOUT_TEST_H
#define GRAPHICAL_TOOL_KIT_APPLICATION_LINEARLAYOUT_TEST_H

#include <Application/Application.h>

class LinearLayoutTest : public Application {
public:
    virtual void onCreateViews();
};

MAKE_APPLICATION_PUBLIC__HEADER(LinearLayoutTest);

#endif //GRAPHICAL_TOOL_KIT_APPLICATION_LINEARLAYOUT_TEST_H
