//
// Created by Matthew Good on 9/8/21.
//

#ifndef GRAPHICAL_TOOL_KIT_VIEWGROUP_H
#define GRAPHICAL_TOOL_KIT_VIEWGROUP_H

#include "View.h"

class ViewGroup : public View {
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
    long identity = 0;
    
public:

    static inline ViewGroup * getParent(View * view) {
        return castToViewType<ViewGroup>(view->parent);
    }

    static inline void detachViewFromParent(View * view) {
        getParent(view)->detachView(view);
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

    void setDiligentAppBase(DiligentAppBase *diligentAppBase) override final;
    void createPipeline(PipelineManager & pipelineManager) override final;
    void destroyPipeline(PipelineManager & pipelineManager) override final;
    void resize(PipelineManager & pipelineManager) override final;
    void create() override final;
    void destroy() override final;
    bool hasPhysics() override final;
    void physics(const TimeEngine & timeEngine) override final;

    virtual void onCreatePipeline(PipelineManager & pipelineManager);
    virtual void onResize(PipelineManager & pipelineManager);
    virtual void onDestroyPipeline(PipelineManager & pipelineManager);
    virtual void onCreate();
    virtual bool onTouchEvent(MultiTouch &touch) override;
    virtual void onDestroy();

    size_t getChildCount() const;

    View * viewAt(size_t index) const;
    AutoResizingArray<View *> getChildren() const;

    virtual void drawRenderTarget(DrawTools &tools, Diligent::IDeviceContext *deviceContext);

    virtual void addView(View * view);
    virtual void removeView(View * view);
    virtual void detachView(View * view);
};


#endif //GRAPHICAL_TOOL_KIT_VIEWGROUP_H
