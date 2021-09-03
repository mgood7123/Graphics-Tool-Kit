//
// Created by Matthew Good on 26/8/21.
//

#include "cube.h"
#include <Views/CubeView.h>

MAKE_APPLICATION_PUBLIC__SOURCE(cube);

void cube::onCreateViews() {
    setContentView(new CubeView());
}