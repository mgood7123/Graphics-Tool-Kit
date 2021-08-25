//
// Created by Matthew Good on 25/8/21.
//

#ifndef GRAPHICAL_TOOL_KIT_APPLICATION_H
#define GRAPHICAL_TOOL_KIT_APPLICATION_H

#include <Views/Views.h>

class Application {
    View *contentView = nullptr;
    DiligentAppBase *diligentAppBase = nullptr;
    TimeEngine timeEngine;

    PipelineManager pipelineManager;
    PixelToNDC pixelToNDC;
    int width;
    int height;
    static constexpr const char *PIPELINE_KEY = "AppInstance RT";
    RenderTarget screen;
    RenderTarget rt;

    // protect against createState and destroyState being called multiple times
    std::atomic_bool destroyed{true};
    std::atomic_bool createCalled{false};

public:
    Application();

    // Virtual destructor makes sure all derived classes can be destroyed
    // through the pointer to the base class
    virtual ~Application();

    void createState();

    void
    onResize(int w, int h, Diligent::ISwapChain *swapChain, Diligent::IRenderDevice *renderDevice);

    void onDraw(Diligent::IDeviceContext *deviceContext);

    bool onTouchEvent(MultiTouch &touch);

    void destroyState();

    void setDiligentAppBase(DiligentAppBase *base);

    virtual void onCreateViews();

    virtual void onDestroyViews();

    void setContentView(View *view);
};

class ApplicationInstance {
public:
    Application * app = nullptr;
    typedef ApplicationInstance* Application_Creator_T();
    typedef void Application_Destroyer_T(ApplicationInstance*);
    Application_Destroyer_T * destroyerT = nullptr;

    ApplicationInstance(Application * application);

    ApplicationInstance(const ApplicationInstance &x) = delete;

    ApplicationInstance(ApplicationInstance &&x) = delete;

    ApplicationInstance & operator=(const ApplicationInstance &x) = delete;

    ApplicationInstance & operator=(ApplicationInstance &&x) = delete;
};

#define MAKE_APPLICATION_PUBLIC__HEADER(name) extern "C" ApplicationInstance * newInstance__##name(); extern "C" void deleteInstance__##name(ApplicationInstance * instance);

#define MAKE_APPLICATION_PUBLIC__SOURCE(name) ApplicationInstance * newInstance__##name() { return new ApplicationInstance(new name()); }; void deleteInstance__##name(ApplicationInstance * instance) { if (instance == nullptr) { Log::Error_And_Throw("attempting to destroy a nullptr instance"); }; if (instance->app == nullptr) { Log::Error_And_Throw("attempting to destroy a nullptr application"); }; delete instance->app; delete instance; };

#endif //GRAPHICAL_TOOL_KIT_APPLICATION_H
