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

public:
    bool printLogging = true;

    View * parent = nullptr;
    bool focus = false;
    
    static const Rectangle ZERO_POSITION;
    static const Rectangle NO_CROPPING;
    static const Rectangle NO_PADDING;
    static const Rectangle NO_MARGIN;

    Rectangle position = ZERO_POSITION;
    Rectangle cropping = NO_CROPPING;
    Rectangle padding = NO_PADDING;
    Rectangle margin = NO_MARGIN;

    template <typename T>
    static inline T * castToViewType(View & view) {
        return dynamic_cast<T*>(&view);
    }

    template <typename T>
    static inline T * castToViewType(View * view) {
        return dynamic_cast<T*>(view);
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
        replaceIfMatches<float, int>(output.bottomRight.x, Rectangle::MATCH_PARENT, width);
        replaceIfMatches<float, int>(output.bottomRight.y, Rectangle::MATCH_PARENT, height);
        return output;
    }

    void setTag(const char * name);
    const char * getTag();

    virtual void setDiligentAppBase(DiligentAppBase * diligentAppBase);
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

    virtual void createPipeline(PipelineManager & pipelineManager);
    virtual void switchToPipeline(PipelineManager & pipelineManager);
    virtual void bindShaderResources(PipelineManager & pipelineManager);
    virtual void destroyPipeline(PipelineManager & pipelineManager);
    virtual void create();
    virtual void resize(PipelineManager & pipelineManager);
    virtual void draw(DrawTools & drawTools, RenderTarget &screenRenderTarget, RenderTarget & renderTarget);
    virtual bool onTouchEvent(MultiTouch &touch);
    virtual void destroy();
    virtual bool hasPhysics();
    virtual void physics(const TimeEngine & timeEngine);

    // delete called on non-final that has virtual functions but non-virtual destructor
    // make virtual default to fix
    virtual ~View() = default;

    static double degreesToRadians(double y);
};

#endif //GRAPHICAL_TOOL_KIT_VIEW_H

#include "ViewGroup.h"
