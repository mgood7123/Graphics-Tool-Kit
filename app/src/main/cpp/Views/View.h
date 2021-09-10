//
// Created by Matthew Good on 30/6/21.
//

#ifndef GRAPHICAL_TOOL_KIT_VIEW_H
#define GRAPHICAL_TOOL_KIT_VIEW_H

#include <DiligentHelpers/DiligentAppBase.h>
#include <VertexEngine/VertexEngine.h>
#include <MultiTouch/MultiTouch.h>
#include "Rectangle.h"

class View {
    static constexpr const char * NO_TAG = "<INTERNAL_VIEW__NO_TAG>";
    const char * tag = NO_TAG;

    DiligentAppBase * diligentAppBase = nullptr;
    
    /**
     * if (value == match) value = replacement;
     */
    template <typename A, typename B>
    void replaceIfMatches(A & value, const B & match, const A & replacement) {
        if (value == match) value = replacement;
    }

    Rectangle relativePosition;
    Rectangle relativePositionPadding;
    Rectangle absolutePosition;

    static Position INVALID_MEASUREMENT_DIMENSION;
    Position measuredDimension;

private:

    /**
     * this is meant to be called before `buildAbsoluteCoordinatesFromRelativeCoordinates`, please use `buildCoordinates` instead
     * @param childInfo the childInfo to store data into
     * @param drawPosition the draw position within the virtual canvas
     * @param drawTools the virtual canvas
     * @param renderTarget the render target to obtain info from if the input childInfo has no parent
     * @return the parent ChildInfo* of the input ChildInfo, otherwise nullptr
     */
    void buildRelativeCoordinatesFromVirtualCoordinates(
            const Rectangle &drawPosition,
            const DrawTools &drawTools,
            const RenderTarget &renderTarget
    );

    /**
     * this is meant to be called after `buildRelativeCoordinatesFromVirtualCoordinates`, please use `buildCoordinates` instead
     */
    void buildAbsoluteCoordinatesFromRelativeCoordinates();
    
    Rectangle cache_layoutData;
    

public:
    class MeasureSpec {
    public:
        enum Mode {
            MATCH_PARENT = -1,
            WRAP_CONTENT = -2,
            INVALID = -3
        };
    };
    Rectangle layoutData;

    bool printLogging = false;

    View * parent = nullptr;
    bool focus = false;
    
    static const Rectangle ZERO_POSITION;
    static const Rectangle NO_CROPPING;
    static const Rectangle NO_PADDING;
    static const Rectangle NO_MARGIN;

    Position maximumSize {MeasureSpec::MATCH_PARENT, MeasureSpec::MATCH_PARENT};

    Rectangle position = ZERO_POSITION;
    Rectangle cropping = NO_CROPPING;
    Rectangle padding = NO_PADDING;
    Rectangle margin = NO_MARGIN;

    template <typename T>
    inline T * castToViewType() {
        return dynamic_cast<T*>(this);
    }

    template <typename T>
    static inline T * castToViewType(View & view) {
        return dynamic_cast<T*>(&view);
    }

    template <typename T>
    static inline T * castToViewType(View * view) {
        return dynamic_cast<T*>(view);
    }

    template <typename T>
    inline bool isInstanceOf() {
        return castToViewType<T>(this) != nullptr;
    }

    template <typename T>
    static inline bool isInstanceOf(View & view) {
        return castToViewType<T>(view) != nullptr;
    }

    template <typename T>
    static inline bool isInstanceOf(View * view) {
        return castToViewType<T>(view) != nullptr;
    }
    
    inline
    Rectangle replace_MATCH_PARENT_with(const Rectangle & input, const int & width, const int & height) {
        Rectangle output = input;
        replaceIfMatches<float, int>(output.bottomRight.x, MeasureSpec::MATCH_PARENT, width);
        replaceIfMatches<float, int>(output.bottomRight.y, MeasureSpec::MATCH_PARENT, height);
        return output;
    }

    void setTag(const char * name);
    const char * getTag();

    virtual void setDiligentAppBase(DiligentAppBase * pDiligentAppBase);
    DiligentAppBase & getDiligentAppBase();

    Rectangle getAbsolutePosition();
    Rectangle getRelativePosition();

    /**
     * @param drawPosition the draw position within the virtual canvas
     * @param drawTools the virtual canvas
     * @param renderTarget the render target to obtain info from if the input childInfo has no parent
     */
    void buildCoordinates(
            const Rectangle & drawPosition,
            const DrawTools & drawTools,
            const RenderTarget & renderTarget
    );

    MultiTouch::TouchData transformTouch(
            const MultiTouch::TouchData & touch, float newWidth, float newHeight
    );

    /**
     * if value is MATCH_PARENT, return max
     * else if value is greater than max, return max
     * else return value
     */
    static constexpr inline float minmax(float value, float max) {
        return value == MeasureSpec::MATCH_PARENT ? max : value > max ? max : value;
    }

    virtual void createPipeline(PipelineManager & pipelineManager);
    virtual void switchToPipeline(PipelineManager & pipelineManager);
    virtual void bindShaderResources(PipelineManager & pipelineManager);
    virtual void destroyPipeline(PipelineManager & pipelineManager);
    virtual void create();
    virtual void resize(PipelineManager & pipelineManager);
    void measure();
    virtual void onMeasure();
    void setMeasuredDimensions(float width, float height);
    Position getMeasuredDimensions();
    float getMeasuredWidth();
    float getMeasuredHeight();
    void layout(const Rectangle &dimensions, DrawTools &drawTools, RenderTarget &screenRenderTarget, RenderTarget &renderTarget);
    virtual void onLayout(bool changed, const Rectangle &dimensions, DrawTools &drawTools, RenderTarget &screenRenderTarget, RenderTarget &renderTarget);
    virtual void draw(DrawTools & drawTools, RenderTarget &screenRenderTarget, RenderTarget & renderTarget);
    virtual bool onTouchEvent(MultiTouch &touch);
    virtual void destroy();
    virtual bool hasPhysics();
    virtual void physics(const TimeEngine & timeEngine);

    // Virtual destructor makes sure all derived classes can be destroyed
    // through the pointer to the base class
    virtual ~View() = default;

    static double degreesToRadians(double y);
};

#define ALLOCATE_VIEW(name, type) type * name = new type(); name->setTag(#name)

#endif //GRAPHICAL_TOOL_KIT_VIEW_H

#include "ViewGroup.h"
