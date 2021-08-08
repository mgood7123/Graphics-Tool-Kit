//
// Created by Matthew Good on 8/8/21.
//

#ifndef GRAPHICAL_TOOL_KIT_DRAWTOOLS_H
#define GRAPHICAL_TOOL_KIT_DRAWTOOLS_H

#include <PixelToNDC/PixelToNDC.h>
#include "PipelineManager.h"

class DrawTools {
public:
    PipelineManager & pipelineManager;
    PixelToNDC & pixelToNDC;
    DrawTools(PipelineManager & pipelineManager, PixelToNDC & pixelToNDC);
};


#endif //GRAPHICAL_TOOL_KIT_DRAWTOOLS_H
