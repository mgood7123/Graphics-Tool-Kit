//
// Created by Matthew Good on 30/6/21.
//

#include "View.h"

const Rectangle View::ZERO_POSITION = {0, 0, Rectangle::MATCH_PARENT, Rectangle::MATCH_PARENT};
const Rectangle View::NO_CROPPING = {0, 0, 0, 0};
const Rectangle View::NO_PADDING = {0, 0, 0, 0};
const Rectangle View::NO_MARGIN = {0, 0, 0, 0};

void View::setTag(const char *name) {
    tag = name;
}

const char * View::getTag() {
    return tag;
}

void View::buildRelativeCoordinatesFromVirtualCoordinates(const Rectangle &drawPosition,
                                                          const DrawTools &drawTools,
                                                          const RenderTarget &renderTarget) {
    int w = 0;
    int h = 0;

    if (parent != nullptr) {
        absolutePosition = parent->absolutePosition;
        relativePosition = absolutePosition;
        relativePosition.bottomRight -= relativePosition.topLeft;
        relativePosition.topLeft = 0;

        w = relativePosition.bottomRight.x;
        h = relativePosition.bottomRight.y;
    } else {
        if (position.isMatchParent()) {
            w = renderTarget.getWidth();
            h = renderTarget.getHeight();
        } else {
            w = position.bottomRight.x;
            h = position.bottomRight.y;
        }

        relativePosition = {position.topLeft.x, position.topLeft.y, w, h};
        absolutePosition = relativePosition;
    }
    
    float drawTools_w = drawTools.pixelToNDC.width;
    float drawTools_h = drawTools.pixelToNDC.height;

    // transform window by drawTools

    // optimization:
    //   skip multiplication and division if not needed:
    //     if value is 0, return 0
    //     if value is max, return real max
    //     otherwise compute value

    float padding_x = padding.topLeft.x == 0 ? 0 : padding.topLeft.x == drawTools_w ? w : roundf((padding.topLeft.x / drawTools_w) * w);
    float position_x = drawPosition.topLeft.x == 0 ? 0 : drawPosition.topLeft.x == drawTools_w ? w : roundf(((drawPosition.topLeft.x + position.topLeft.x) / drawTools_w) * w);

    float padding_y = padding.topLeft.y == 0 ? 0 : padding.topLeft.y == drawTools_h ? h : roundf((padding.topLeft.y / drawTools_h) * h);
    float position_y = drawPosition.topLeft.y == 0 ? 0 : drawPosition.topLeft.y == drawTools_w ? h : roundf(((drawPosition.topLeft.y + position.topLeft.y) / drawTools_h) * h);

    float padding_w = padding.bottomRight.x == 0 ? 0 : padding.bottomRight.x == drawTools_w ? w : roundf((padding.bottomRight.x / drawTools_w) * w);
    float position_w = drawPosition.bottomRight.x == 0 ? 0 : drawPosition.bottomRight.x == drawTools_w ? w : roundf(((drawPosition.bottomRight.x +
                                (position.bottomRight.x == Rectangle::MATCH_PARENT ? 0
                                : position.bottomRight.x)) /
                               drawTools_w) * w);

    float padding_h = padding.bottomRight.y == 0 ? 0 : padding.bottomRight.y == drawTools_h ? h : roundf((padding.bottomRight.y / drawTools_h) * h);
    float position_h = drawPosition.bottomRight.y == 0 ? 0 : drawPosition.bottomRight.y == drawTools_h ? h : roundf(((drawPosition.bottomRight.y +
                                (position.bottomRight.y == Rectangle::MATCH_PARENT ? 0
                                : position.bottomRight.y)) /
                               drawTools_h) * h);

    relativePositionPadding = {padding_x, padding_y, padding_w, padding_h};
    relativePosition = {
            position_x,
            position_y,
            position_w,
            position_h
    };

    if (printLogging) {
        Log::Info("TAG: ", getTag(), ", relative position         : ", relativePosition);
        Log::Info("TAG: ", getTag(), ", relative position padding : ", relativePositionPadding);
    }
}

void View::buildAbsoluteCoordinatesFromRelativeCoordinates() {
    // lots of help from Akronymus from libera-chat ##programming

    if (parent != nullptr) {
        absolutePosition.topLeft = parent->absolutePosition.topLeft + relativePosition.topLeft;
    } else {
        absolutePosition.topLeft = relativePosition.topLeft;
    }

    absolutePosition.bottomRight = absolutePosition.topLeft + relativePosition.bottomRight;

    absolutePosition.topLeft += relativePositionPadding.topLeft;
    absolutePosition.bottomRight -= relativePositionPadding.bottomRight;

    if (printLogging) {
        Log::Info("TAG: ", getTag(), ", absolute position         : ", absolutePosition);
    }
}

void View::buildCoordinates(const Rectangle &drawPosition,
                                 const DrawTools &drawTools,
                                 const RenderTarget &renderTarget) {
    buildRelativeCoordinatesFromVirtualCoordinates(drawPosition, drawTools, renderTarget);
    buildAbsoluteCoordinatesFromRelativeCoordinates();
}

void View::setDiligentAppBase(DiligentAppBase * diligentAppBase) {
    this->diligentAppBase = diligentAppBase;
}

DiligentAppBase &View::getDiligentAppBase() {
    if (diligentAppBase == nullptr) {
        Log::Error_And_Throw("diligentAppBase has not been set");
    }
    return *diligentAppBase;
}

Rectangle View::getAbsolutePosition() {
    return absolutePosition;
}

Rectangle View::getRelativePosition() {
    return relativePosition.resizeBy(relativePositionPadding);
}

void View::createPipeline(PipelineManager & pipelineManager) {}

void View::switchToPipeline(PipelineManager & pipelineManager) {}

void View::bindShaderResources(PipelineManager & pipelineManager) {}

void View::destroyPipeline(PipelineManager & pipelineManager) {}

void View::create()
{}

void View::resize(PipelineManager & pipelineManager)
{}

void View::draw(DrawTools & drawTools, RenderTarget & screenRenderTarget, RenderTarget & renderTarget)
{}

bool View::onTouchEvent(MultiTouch &multiTouch) {
    return false;
}

void View::destroy ()
{}

bool View::hasPhysics()
{
    return false;
}

void View::physics (const TimeEngine & timeEngine)
{}

double View::degreesToRadians(double y)
{
    return y * (M_PI/180);
}
