//
// Created by Matthew Good on 26/8/21.
//

#ifndef GRAPHICAL_TOOL_KIT_APPLICATION_CUBE_H
#define GRAPHICAL_TOOL_KIT_APPLICATION_CUBE_H


#include <Application/Application.h>

class cube : public Application {
public:
    virtual void onCreateViews();
};

MAKE_APPLICATION_PUBLIC__HEADER(cube);

#endif //GRAPHICAL_TOOL_KIT_APPLICATION_CUBE_H
