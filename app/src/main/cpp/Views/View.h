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

    Rectangle drawPosition;
    Position drawDimensions;
    Rectangle relativePosition;
    Rectangle relativePositionPadding;
    Rectangle absolutePosition;

    static Position INVALID_MEASUREMENT_DIMENSION;
    Position measuredDimension;

    /**
     * this is meant to be called before `buildAbsoluteCoordinatesFromRelativeCoordinates`, please use `buildCoordinates` instead
     * @param virtualViewport the draw position within the virtual canvas
     * @param drawTools the virtual canvas
     * @param renderTarget the render target to obtain info from if the input childInfo has no parent
     */
    void buildRelativeCoordinatesFromVirtualCoordinates(
            const Rectangle &virtualViewport,
            const DrawTools &drawTools,
            const RenderTarget &renderTarget
    );

    /**
     * this is meant to be called after `buildRelativeCoordinatesFromVirtualCoordinates`, please use `buildCoordinates` instead
     */
    void buildAbsoluteCoordinatesFromRelativeCoordinates();
    
    Rectangle cache_layoutData;

    // this has a default size of 100, 100
    Position virtualCanvas = {100, 100};

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
    
    static const Position NO_OFFSET;
    static const Position NO_SIZE;
    static const Rectangle NO_CROPPING;
    static const Rectangle NO_PADDING;
    static const Rectangle NO_MARGIN;

    // offset is defined as the amount the view is shifted inside the canvas
    // eg
    //  in a canvas size of 50
    //   position this view at 10, 10
    //
    Position offsetInVirtualCanvas = NO_OFFSET;
    
    // size is defined as width and height
    // eg
    //  in a canvas size of 50
    //   give this view a width of 20 and a height of 30
    //
    Position sizeInVirtualCanvas = NO_SIZE;

    Rectangle croppingInVirtualCanvas = NO_CROPPING;

    // padding is defined as a type of position that resizes the view
    // eg
    //  in a canvas size of 50
    //   position this view at 10, 10, with a width of 30 and height of 30
    //   and pad this view with padding position of 5, 5, with a width of 5 and height of 5
    //
    //     this means that this view will be positioned at
    //       15, 15, with a width of 20 and a height of 20
    //
    //  padding is calculated as follows
    //
    //    pad the position:
    //      pos.topLeft.x += padding.topLeft.x         // x
    //      pos.topLeft.y += padding.topLeft.y         // y
    //
    //    shrink the width and height to match the padded position:
    //      pos.bottomRight.x -= padding.topLeft.x     // x
    //      pos.bottomRight.y -= padding.topLeft.y     // y
    //
    //    pad the width and height
    //      pos.bottomRight.x -= padding.bottomRight.x // width
    //      pos.bottomRight.y -= padding.bottomRight.y // height
    //
    Rectangle paddingInVirtualCanvas = NO_PADDING;

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
    Rectangle replace_MATCH_PARENT_with(const Rectangle & input, const Position & size) {
        return replace_MATCH_PARENT_with(input, size.x, size.y);
    }

    inline
    Rectangle replace_MATCH_PARENT_with(const Rectangle & input, const int & width, const int & height) {
        Rectangle output = input;
        replaceIfMatches<float, float>(output.bottomRight.x, MeasureSpec::MATCH_PARENT, width);
        replaceIfMatches<float, float>(output.bottomRight.y, MeasureSpec::MATCH_PARENT, height);
        return output;
    }

    inline
    Position replace_MATCH_PARENT_with(const Position & input, const Position & size) {
        return replace_MATCH_PARENT_with(input, size.x, size.y);
    }

    inline
    Position replace_MATCH_PARENT_with(const Position & input, const int & width, const int & height) {
        Position output = input;
        replaceIfMatches<float, float>(output.x, MeasureSpec::MATCH_PARENT, width);
        replaceIfMatches<float, float>(output.y, MeasureSpec::MATCH_PARENT, height);
        return output;
    }

    void setTag(const char * name);
    const char * getTag();

    virtual void setDiligentAppBase(DiligentAppBase * pDiligentAppBase);
    DiligentAppBase & getDiligentAppBase();

    void setVirtualCanvasSize(const int & width, const int & height);
    void setVirtualCanvasSize(const int & width, const float & height);
    void setVirtualCanvasSize(const float & width, const int & height);
    void setVirtualCanvasSize(const float & width, const float & height);
    void setVirtualCanvasSize(const Position & size);
    Position getVirtualCanvasSize();

    Rectangle getDrawPosition();
    Position getDrawDimensions();
    Rectangle getAbsolutePosition();
    Rectangle getRelativePosition();
    Rectangle getRelativePositionPadding();

    /**
     * @param virtualViewport the draw position within the virtual canvas
     * @param drawTools the virtual canvas
     * @param renderTarget the render target to obtain info from if the input childInfo has no parent
     */
    void buildCoordinates(
            const Rectangle & virtualViewport,
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
    
    /**
     * if value is MATCH_PARENT, return max
     * else if value is greater than max, return max
     * else return value
     */
    static constexpr inline Position minmax(Position value, Position max) {
        return {
            minmax(value.x, max.x),
            minmax(value.y, max.y)
        };
    }

    /**
     * if value is MATCH_PARENT, return max
     * else if value is greater than max, return max
     * else return value
     */
    static constexpr inline Rectangle minmax(Rectangle value, Rectangle max) {
        return {
            minmax(value.topLeft, max.topLeft),
            minmax(value.bottomRight, max.bottomRight)
        };
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
