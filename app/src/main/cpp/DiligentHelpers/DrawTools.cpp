//
// Created by Matthew Good on 8/8/21.
//

#include "DrawTools.h"

DrawTools::DrawTools(
        PipelineManager &pipelineManager, PixelToNDC &pixelToNDC
) : pipelineManager(pipelineManager), pixelToNDC(pixelToNDC) {}
