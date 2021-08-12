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
    
    int resizeWidth = 0;
    int resizeHeight = 0;
public:
    class ChildInfo {
    public:
        Position positionInWindow;
        ObjectBase * object;
        
    private:
        bool deallocateOnDestruction;

    public:
        inline
        ChildInfo(const Position & positionInWindow, ObjectBase * object, bool deallocateOnDestruction):
        positionInWindow(positionInWindow), object(object),
        deallocateOnDestruction(deallocateOnDestruction) {}

        inline
        ChildInfo(ObjectBase * object, bool deallocateOnDestruction):
        ChildInfo(ZERO_POSITION, object, deallocateOnDestruction) {}
        
        inline
        ChildInfo(const ChildInfo & childInfo) {
            this->positionInWindow = childInfo.positionInWindow;
            this->object = childInfo.object;
            if (childInfo.deallocateOnDestruction) {
                deallocateOnDestruction = true;
                ChildInfo & child = const_cast<ChildInfo&>(childInfo);
                child.object = nullptr;
                child.deallocateOnDestruction = false;
            }
        }

        inline
        ChildInfo(const ChildInfo & childInfo, bool deallocateOnDestruction):
        positionInWindow(childInfo.positionInWindow), object(childInfo.object),
        deallocateOnDestruction(deallocateOnDestruction) {
            if (deallocateOnDestruction) {
                ChildInfo & child = const_cast<ChildInfo&>(childInfo);
                child.object = nullptr;
                child.deallocateOnDestruction = false;
            }
        }

        inline
        ChildInfo(ChildInfo && childInfo) {
            this->positionInWindow = childInfo.positionInWindow;
            this->object = childInfo.object;
            if (childInfo.deallocateOnDestruction) {
                deallocateOnDestruction = true;
                childInfo.object = nullptr;
                childInfo.deallocateOnDestruction = false;
            }
        }

        inline
        ChildInfo & operator=(const ChildInfo & childInfo) {
            this->positionInWindow = childInfo.positionInWindow;
            this->object = childInfo.object;
            if (childInfo.deallocateOnDestruction) {
                deallocateOnDestruction = true;
                ChildInfo & child = const_cast<ChildInfo&>(childInfo);
                child.object = nullptr;
                child.deallocateOnDestruction = false;
            }
            return *this;
        }
        
        inline
        ChildInfo & operator=(ChildInfo && childInfo) {
            this->positionInWindow = childInfo.positionInWindow;
            this->object = childInfo.object;
            if (childInfo.deallocateOnDestruction) {
                deallocateOnDestruction = true;
                childInfo.object = nullptr;
                childInfo.deallocateOnDestruction = false;
            }
            return *this;
        }
        
        inline
        ~ChildInfo() {
            if (deallocateOnDestruction) delete object;
        }
    };
    
    static inline void detachObjectFromParent(ObjectBase * object) {
        static_cast<ObjectGroup *>(object->parent)->detachChild(object);
    }
    
    inline Position transformToWindowCoordinates(
        const Position & copy, const Position & position, ObjectBase * obj, const DrawTools & drawTools, const RenderTarget & renderTarget
    ) {
        Position position_ = obj->applyPadding(position);
        
        float draw_w = copy.width + position_.x;
        float draw_h = copy.height + position_.y;

        float drawTools_w = drawTools.pixelToNDC.width;
        float drawTools_h = drawTools.pixelToNDC.height;

        float window_width = renderTarget.getWidth();
        float window_height = renderTarget.getHeight();
        
        // transform window by drawTools
        
        float percent_drawTools_w = draw_w / drawTools_w;
        float percent_drawTools_h = draw_h / drawTools_h;
        
        float transformed_window_w = window_width * percent_drawTools_w;
        float transformed_window_h = window_height * percent_drawTools_h;
        
        // transform draw region by transformed window

        float x = copy.x + obj->cropping.x + position_.x;
        float y = copy.y + obj->cropping.y + position_.y;
        float w = (copy.width + position_.x) - obj->cropping.width;
        float h = (copy.height + position_.y) - obj->cropping.height;
        
        float percent_draw_region_x = x / draw_w;
        float percent_draw_region_y = y / draw_h;
        float percent_draw_region_w = w / draw_w;
        float percent_draw_region_h = h / draw_h;
        
        float transformed_window_region_x = transformed_window_w * percent_draw_region_x;
        float transformed_window_region_y = transformed_window_h * percent_draw_region_y;
        float transformed_window_region_w = transformed_window_w * percent_draw_region_w;
        float transformed_window_region_h = transformed_window_h * percent_draw_region_h;

        return {
            static_cast<int>(transformed_window_region_x),
            static_cast<int>(transformed_window_region_y),
            static_cast<int>(transformed_window_region_w),
            static_cast<int>(transformed_window_region_h)
        };
    }
    
    
    inline MultiTouch::TouchData transformTouchCoordinatesToChildCoordinates(
        const MultiTouch::TouchData & touch, float currentWidth, float currentHeight, ChildInfo * childInfo
    ) {
        return transformTouchCoordinates(
            touch,
            currentWidth, currentHeight,
            childInfo->positionInWindow.width,
            childInfo->positionInWindow.height
        );
    }

    inline MultiTouch::TouchData transformTouchCoordinates(
        const MultiTouch::TouchData & touch, float currentWidth, float currentHeight, float newWidth, float newHeight
    ) {
        return {
            touch.identity,
            (touch.x/currentWidth)*newWidth,
            (touch.y/currentHeight)*newHeight,
            touch.size, touch.pressure,
            touch.state, touch.moved
        };
    }

    inline void drawAndClipToBoundaries(
            ChildInfo * child,
            RenderTarget & renderTarget, DrawTools & drawTools,
            const Position & position,
            Diligent::IDeviceContext * deviceContext
    ) {
        ObjectBase * obj = child->object;

        Position position_ = obj->applyPadding(position);
        auto copy = replace_MATCH_PARENT_with(obj->position, position_.width, position_.height);

        child->positionInWindow = transformToWindowCoordinates(copy, position, obj, drawTools, renderTarget);
        
        renderTarget.clip(child->positionInWindow, deviceContext);
        renderTarget.draw(
                drawTools,
                copy.x + position_.x,
                copy.y + position_.y,
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

    AutoResizingArray<ChildInfo *> getChildren() const;

    void addChild(ObjectBase * obj);
    void addChild(ChildInfo *childInfo);
    void addChild(const ChildInfo &childInfo);
    void removeChild(ObjectBase * obj);
    void removeChild(ChildInfo *childInfo);
    void removeChild(ChildInfo &childInfo);
    void detachChild(ObjectBase * obj);
    void detachChild(ChildInfo *childInfo);
    void detachChild(ChildInfo &childInfo);
};


#endif //GRAPHICAL_TOOL_KIT_OBJECTGROUP_H
