//
// Created by Matthew Good on 25/8/21.
//

#ifndef GRAPHICAL_TOOL_KIT_LIFECYCLE_H
#define GRAPHICAL_TOOL_KIT_LIFECYCLE_H

#include "ApplicationLoader.h"

class LifeCycle {
    ApplicationLoader::ApplicationInstancer * applicationInstancer = nullptr;
    ApplicationInstance *application = nullptr;
    std::atomic_bool destroyed{true};
    std::atomic_bool createCalled{false};
    std::atomic_bool canResize{false};
    Diligent::ISwapChain *savedSwapChain = nullptr;
    Diligent::IRenderDevice *savedRenderDevice = nullptr;
    int savedW = 0;
    int savedH = 0;

public:

    ~LifeCycle();
    
    void setApplication(const char * name, ApplicationLoader::ApplicationInstancer * appInstancer, DiligentAppBase *diligentAppBase);

    void setApplication(ApplicationInstance *appInstance, DiligentAppBase *diligentAppBase);

    void setApplication(Application *application, DiligentAppBase *diligentAppBase);

    void deleteApplication();
    
    void createState();

    void destroyState();

    void
    onResize(int w, int h, Diligent::ISwapChain *swapChain, Diligent::IRenderDevice *renderDevice);

    bool isDestroyed();

    void onDraw(Diligent::IDeviceContext *deviceContext);

    bool onTouchEvent(MultiTouch &touch);

    void onCreateViews();

    void onDestroyViews();

    void create();

    void destroy();
};

#endif //GRAPHICAL_TOOL_KIT_LIFECYCLE_H
