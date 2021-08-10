//
// Created by Matthew Good on 30/6/21.
//

#ifndef GRAPHICAL_TOOL_KIT_OBJECTBASE_H
#define GRAPHICAL_TOOL_KIT_OBJECTBASE_H

#include <DiligentHelpers/DiligentAppBase.h>
#include <VertexEngine/VertexEngine.h>
#include <MultiTouch/MultiTouch.h>
#include "Position.h"

class ObjectBase {
    static constexpr const char * NO_TAG = "<INTERNAL_OBJECT_BASE__NO_TAG>";

    DiligentAppBase * diligentAppBase = nullptr;
    const char * tag = NO_TAG;
    
    /**
     * if (value == match) value = replacement;
     */
    template <typename A, typename B>
    void replaceIfMatches(A & value, const B & match, const A & replacement) {
        if (value == match) value = replacement;
    }
    
public:
    
    static const Position ZERO_POSITION;
    static const Position NO_CROPPING;
    static const Position NO_PADDING;
    static const Position NO_MARGIN;

    Position position = ZERO_POSITION;
    Position cropping = NO_CROPPING;
    Position padding = NO_PADDING;
    Position margin = NO_MARGIN;
    
    inline
    Position replace_MATCH_PARENT_with(const Position & input, const int & width, const int & height) {
        Position output = input;
        replaceIfMatches(output.width, Position::MATCH_PARENT, width);
        replaceIfMatches(output.height, Position::MATCH_PARENT, height);
        return output;
    }

    inline Position applyPadding(const Position & input) {
        return {
            input.x + padding.x,
            input.y + padding.y,
            input.width - padding.x - padding.width ,
            input.height - padding.y - padding.height
        };
    }

    void setTag(const char * name);
    const char * getTag();

    virtual void setDiligentAppBase(DiligentAppBase * diligentAppBase);
    DiligentAppBase & getDiligentAppBase();

    virtual void createPipeline(PipelineManager & pipelineManager);
    virtual void switchToPipeline(PipelineManager & pipelineManager);
    virtual void bindShaderResources(PipelineManager & pipelineManager);
    virtual void destroyPipeline(PipelineManager & pipelineManager);
    virtual void create();
    virtual void resize(PipelineManager & pipelineManager);
    virtual void draw(DrawTools & drawTools, RenderTarget & renderTarget);
    virtual bool onTouchEvent(MultiTouch &touch);
    virtual void destroy();
    virtual bool hasPhysics();
    virtual void physics(const TimeEngine & timeEngine);
    // delete called on non-final that has virtual functions but non-virtual destructor
    virtual ~ObjectBase() = default;

    static double degreesToRadians(double y);
};


#endif //GRAPHICAL_TOOL_KIT_OBJECTBASE_H
