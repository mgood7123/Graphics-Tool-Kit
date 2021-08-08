//
// Created by Matthew Good on 8/8/21.
//

#include "PixelToNDC.h"

PixelToNDC::PixelToNDC(int width, int height) {
    resize(width, height);
}

void PixelToNDC::resize(int width, int height) {
    this->width = width;
    this->height = height;
}
