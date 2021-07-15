//
// Created by Matthew Good on 15/7/21.
//

#include "AppInstanceMacOSBase.h"

void AppInstanceMacOSBase::swapBuffers ()
{
    SDL_GL_SwapWindow(mainwindow);
}