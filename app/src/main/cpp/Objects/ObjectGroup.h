//
// Created by Matthew Good on 9/8/21.
//

#ifndef GRAPHICAL_TOOL_KIT_OBJECTGROUP_H
#define GRAPHICAL_TOOL_KIT_OBJECTGROUP_H

#include "ObjectBase.h"

class ObjectGroup : public ObjectBase {
    // use {} to call constructor
    // using () is reserved for functions
    Kernel children {8};

    PipelineManager *pipelineManagerUsed = nullptr;
    bool setDiligentAppBaseCalled = false;
    bool createPipelineCalled = false;
    bool destroyPipelineCalled = false;
    bool createCalled = false;
    bool destroyCalled = false;
    bool resizeCalled = false;
    bool hasPhysicsCalled = false;
public:

    inline void drawAndClipToBoundaries(
            const ObjectBase * obj,
            RenderTarget & renderTarget, DrawTools & drawTools,
            const Position & position,
            Diligent::IDeviceContext * deviceContext
    ) {
        auto copy = replace_MATCH_PARENT_with(obj->position, position.width, position.height);

        renderTarget.clip(
                {
                        copy.x + obj->cropping.x + position.x,
                        copy.y + obj->cropping.y + position.y,
                        (copy.width + position.x) - obj->cropping.width ,
                        (copy.height + position.y) - obj->cropping.height
                },
                drawTools,
                copy.width + position.x,
                copy.height + position.y,
                deviceContext
        );
        renderTarget.draw(
                drawTools,
                copy.x + position.x,
                copy.y + position.y,
                copy.width, copy.height,
                deviceContext
        );
    }

    void setDiligentAppBase(DiligentAppBase *diligentAppBase) override final;
    bool hasPhysics() override final;
    void physics(const TimeEngine & timeEngine) override final;

    virtual void createPipeline(PipelineManager & pipelineManager) override;
    virtual void destroyPipeline(PipelineManager & pipelineManager) override;
    virtual void create() override;
    virtual void resize(PipelineManager & pipelineManager) override;
    virtual bool onTouchEvent(MultiTouch &touch) override;
    virtual void destroy() override;

    size_t getChildCount() const;

    AutoResizingArray<ObjectBase *> getChildren() const;

    void addChild(ObjectBase * obj);
    void removeChild(ObjectBase * obj);

    ~ObjectGroup();;
};


#endif //GRAPHICAL_TOOL_KIT_OBJECTGROUP_H
