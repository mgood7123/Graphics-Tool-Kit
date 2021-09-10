//
// Created by Matthew Good on 26/8/21.
//

#ifndef GRAPHICAL_TOOL_KIT_APPLICATION_TEXTVIEW_TEST_H
#define GRAPHICAL_TOOL_KIT_APPLICATION_TEXTVIEW_TEST_H

#include <Application/Application.h>

class TextViewTest : public Application {
public:
    virtual void onCreateViews();
};

MAKE_APPLICATION_PUBLIC__HEADER(TextViewTest);

#endif //GRAPHICAL_TOOL_KIT_APPLICATION_TEXTVIEW_TEST_H
