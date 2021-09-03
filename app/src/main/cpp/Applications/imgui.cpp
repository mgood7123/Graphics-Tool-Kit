//
// Created by Matthew Good on 26/8/21.
//

#include "imgui.h"

MAKE_APPLICATION_PUBLIC__SOURCE(imgui);

void imgui::onCreateViews() {
    setContentView(new TextView());
}
