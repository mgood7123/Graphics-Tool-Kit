//
// Created by Matthew Good on 9/8/21.
//

#ifndef GRAPHICAL_TOOL_KIT_VIEWGROUP_H
#define GRAPHICAL_TOOL_KIT_VIEWGROUP_H

#include "View.h"

// Note: Compose UI does not permit multi-pass measurement.
// This means that a layout element may not measure any of its children more than once
// in order to try different measurement configurations.

// Single-pass measurement is good for performance,
// allowing Compose to efficiently handle deep UI trees.
// If an element measured its child twice and that child measured one of its children twice and so on,
// a single attempt to lay out a whole UI would have to do a lot of work,
// making it hard to keep your app performing well.

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

    typedef std::pair<View*, MultiTouch> TrackingData;
    // use {} to call constructor
    // using () is reserved for functions
    Kernel tracked_views{2};

    RenderTarget rt;
    RenderTarget rt2;
    
public:

    bool log_TouchEvents = false;
    bool log_TouchEnterExit = false;
    bool log_TouchPointers = false;
    bool log_TouchPosition = false;

    static inline ViewGroup * getParent(View * view) {
        return castToViewType<ViewGroup>(view->parent);
    }

    static inline void detachViewFromParent(View * view) {
        getParent(view)->detachView(view);
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

    View * getChildAt(size_t index) const;
    AutoResizingArray<View *> getChildren() const;

    virtual void addView(View * view);
    virtual void removeView(View * view);
    virtual void detachView(View * view);

    virtual const char * getPipelineKeyA() = 0;
    virtual const char * getPipelineKeyB() = 0;

    virtual void draw(DrawTools &drawTools, RenderTarget &screenRenderTarget, RenderTarget &renderTarget) override;
};


#endif //GRAPHICAL_TOOL_KIT_VIEWGROUP_H
