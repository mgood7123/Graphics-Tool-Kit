//
// Created by Matthew Good on 30/6/21.
//

#include "View.h"

const Position View::NO_OFFSET = {0, 0};
const Position View::NO_SIZE = {MeasureSpec::MATCH_PARENT, MeasureSpec::MATCH_PARENT};
const Rectangle View::NO_CROPPING = {0, 0, 0, 0};
const Rectangle View::NO_PADDING = {0, 0, 0, 0};

void View::setTag(const char *name) {
    tag = name;
}

const char * View::getTag() {
    return tag;
}

void View::setVirtualCanvasSize(const int & width, const int & height) {
    setVirtualCanvasSize({width, height});
}

void View::setVirtualCanvasSize(const int & width, const float & height) {
    setVirtualCanvasSize({width, height});
}

void View::setVirtualCanvasSize(const float & width, const int & height) {
    setVirtualCanvasSize({width, height});
}

void View::setVirtualCanvasSize(const float & width, const float & height) {
    setVirtualCanvasSize({width, height});
}

void View::setVirtualCanvasSize(const Position & size) {
    virtualCanvas = size;
}

Position View::getVirtualCanvasSize() {
    return virtualCanvas;
}

// TODO: maybe implement Jetpack Compose's Modifier stategy
//  example: Modifier.padding(16).height(10dp); // height 66, content 50, padding 16
//  example: Modifier.height(50dp).padding(16); // height 50, content 34, padding 16
//
// Size 20       (0, 0, 20, 20)
// Pad 2         (2, 2, 16, 16)
// MinHeight 20  Invalid
// available space 14
//
// Vs
//
// Size 20       (0, 0, 20, 20)
// MinHeight 20  (0, 0, 20, 20)
// Pad 2         (2, 2, 16, 16)
// available space 14

void View::buildRelativeCoordinatesFromVirtualCoordinates(const Rectangle &virtualViewport,
                                                          const DrawTools &drawTools,
                                                          const RenderTarget &renderTarget) {
    // coordinates are build from virtual canvas
    //
    // virtual canvas size is specified by the user
    //
    // offset, size, and padding are also specified by the user
    //
    // it is the user's responsibility to make sure that
    //   offset, size, and padding are within acceptable ranges
    //   in regards to virtual canvas size
    //
    // we need to compute two sets of coordinates
    //   draw coordinates - these consume the entire virtual canvas size
    //   relative coordinates - these consume the entire view size

    // calculate drawing coordinates:
    //   drawing coordinates  --- drawPosition.bottomRight

    this->drawPosition = 0;
    drawDimensions = virtualCanvas;
    
    // the drawPosition indicates the position and length of our draw
    //   drawPosition.topLeft = x,y position start
    //   drawPosition.bottomRight = x,y position end
    
    Rectangle virtual_canvas_draw_position = 0;
    
    virtual_canvas_draw_position.bottomRight = replace_MATCH_PARENT_with(sizeInVirtualCanvas, drawDimensions);
    virtual_canvas_draw_position += offsetInVirtualCanvas;

    virtual_canvas_draw_position.topLeft += paddingInVirtualCanvas.topLeft;
    virtual_canvas_draw_position.bottomRight -= paddingInVirtualCanvas.bottomRight;

    Rectangle virtual_canvas_draw_position_percentage = (virtual_canvas_draw_position / drawDimensions);

    this->drawPosition = virtual_canvas_draw_position_percentage * virtualViewport.size();
    this->drawPosition += virtualViewport.topLeft;

    // calculate relative coordinates:
    //   relative coordinates --- w, h
    //

    // we already calculated the percentages above, so no need to recalculate
    
    // scale the resulting drawing position by the upper level size
    // this will be the size of the container that holds this view
    // it may be a parent view, or it may be a GL/Vulkan window
    //   usually the size of the OS window
    //   it may also be the size of smaller GL/Vulkan view
    //     that resides in the UI of the OS window
    
    Position topLevelSize;
    
    if (parent != nullptr) {
        topLevelSize = parent->absolutePosition.size();
    } else {
        topLevelSize = {renderTarget.getWidth(), renderTarget.getHeight()};
    }

    Rectangle relative_percentage = this->drawPosition / drawDimensions;

    relativePosition = relative_percentage * topLevelSize;

    if (printLogging) {
        Log::Info("TAG: ", getTag(), ", virtual viewport                        : ", virtualViewport);
        Log::Info("TAG: ", getTag(), ", draw dimensions                         : ", drawDimensions);
        Log::Info("TAG: ", getTag(), ", offset in virtual canvas                : ", offsetInVirtualCanvas);
        Log::Info("TAG: ", getTag(), ", size in virtual canvas                  : ", sizeInVirtualCanvas);
        Log::Info("TAG: ", getTag(), ", padding in virtual canvas               : ", paddingInVirtualCanvas);
        Log::Info("TAG: ", getTag(), ", virtual_canvas_draw_position            : ", virtual_canvas_draw_position);
        Log::Info("TAG: ", getTag(), ", virtual_canvas_draw_position percentage : ", virtual_canvas_draw_position_percentage);
        Log::Info("TAG: ", getTag(), ", resulting draw position                 : ", this->drawPosition);
        Log::Info("TAG: ", getTag(), ", top level size                          : ", topLevelSize);
        Log::Info("TAG: ", getTag(), ", relative position                       : ", relativePosition);
        Log::Info("TAG: ", getTag(), ", relative percentage                     : ", relative_percentage);
    }
}

void View::buildAbsoluteCoordinatesFromRelativeCoordinates() {
    
    Position parent_TL = parent == nullptr ? 0 : parent->absolutePosition.topLeft;
    absolutePosition = relativePosition + parent_TL;
    
    if (printLogging) {
        Log::Info("TAG: ", getTag(), ", absolute position         : ", absolutePosition);
    }
}

void View::buildCoordinates(const Rectangle &virtualViewport,
                                 const DrawTools &drawTools,
                                 const RenderTarget &renderTarget) {
    buildRelativeCoordinatesFromVirtualCoordinates(virtualViewport, drawTools, renderTarget);
    buildAbsoluteCoordinatesFromRelativeCoordinates();
}

void View::setDiligentAppBase(DiligentAppBase * pDiligentAppBase) {
    this->diligentAppBase = pDiligentAppBase;
}

DiligentAppBase &View::getDiligentAppBase() {
    if (diligentAppBase == nullptr) {
        Log::Error_And_Throw("diligentAppBase has not been set");
    }
    return *diligentAppBase;
}

Rectangle View::getDrawPosition() {
    return drawPosition;
}

Position View::getDrawDimensions() {
    return drawDimensions;
}

Rectangle View::getAbsolutePosition() {
    return absolutePosition;
}

Rectangle View::getRelativePosition() {
    return relativePosition;
}

Rectangle View::getRelativePositionPadding() {
    return relativePositionPadding;
}

void View::createPipeline(PipelineManager & pipelineManager) {}

void View::switchToPipeline(PipelineManager & pipelineManager) {}

void View::bindShaderResources(PipelineManager & pipelineManager) {}

void View::destroyPipeline(PipelineManager & pipelineManager) {}

void View::createState()
{}

void View::resize(PipelineManager & pipelineManager)
{}

void View::onMeasure()
{}

void View::setMeasuredDimensions(float width, float height) {
    measuredDimension = {width, height};
}

Position View::getMeasuredDimensions() {
    return measuredDimension;
}

float View::getMeasuredWidth() {
    return measuredDimension.x;
}

float View::getMeasuredHeight() {
    return measuredDimension.y;
}

Position View::INVALID_MEASUREMENT_DIMENSION = Position(MeasureSpec::INVALID, MeasureSpec::INVALID);

void View::measure()
{
    measuredDimension = INVALID_MEASUREMENT_DIMENSION;
    onMeasure();
    if (measuredDimension == INVALID_MEASUREMENT_DIMENSION) {
        Log::Error_And_Throw("invalid measurement, did you forget to call setMeasuredDimensions(float, float); ?");
    }
}

void View::onLayout(bool changed, const Rectangle &dimensions, DrawTools &drawTools, RenderTarget &screenRenderTarget, RenderTarget &renderTarget)
{}

void View::layout(const Rectangle &dimensions, DrawTools &drawTools, RenderTarget &screenRenderTarget, RenderTarget &renderTarget)
{
    layoutData = dimensions;
    bool changed = false;
    if (cache_layoutData != layoutData) {
        cache_layoutData = layoutData;
        changed = true;
    }
    onLayout(changed, layoutData, drawTools, screenRenderTarget, renderTarget);
}

void View::draw(DrawTools & drawTools, RenderTarget & screenRenderTarget, RenderTarget & renderTarget)
{}

bool View::onTouchEvent(MultiTouch &multiTouch) {
    return false;
}

void View::destroyState()
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

MultiTouch::TouchData
View::transformTouch(const MultiTouch::TouchData &touch, float newWidth, float newHeight) {
    // we need to convert to local in order to account for absolute offset
    // if mouse location is 50, and absolute start is 50, and absolute end is 100
    // then percentage will be 0.5 instead of 0
    Position local = Position(touch.x, touch.y) - absolutePosition.topLeft;
    
    // obtain percentage by dividing by relative size, as relative may be offset
    Position percentage = local / relativePosition.size();
    Position position = percentage * Position(newWidth, newHeight);
    return {
            touch.identity,
            touch.timestamp,
            position.x, position.y,
            touch.size, touch.pressure,
            touch.state, touch.moved
    };
}

View::LayoutParams *View::getLayoutParams() const {
    return layoutParams;
}

void View::setLayoutParams(View::LayoutParams *layoutParams) {
    // important optimization:
    // if layoutParams are self, then
    //   do not delete this->layoutParams
    //     it will invalidate/corrupt the passed layoutParams
    //     since it points this->layoutParams, resulting in use-after-free
    //
    //   avoid this useless assignment:
    //     LayoutParams * layoutParams = this->layoutParams;
    //     this->layoutParams = layoutParams;
    if (layoutParams != this->layoutParams) {
        delete this->layoutParams;
        this->layoutParams = layoutParams;
    }
}

View::~View() {
    delete layoutParams;
}
