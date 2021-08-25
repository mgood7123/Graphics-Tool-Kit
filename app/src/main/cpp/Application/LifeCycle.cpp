//
// Created by Matthew Good on 25/8/21.
//

#include "LifeCycle.h"
#include "Application.h"

void LifeCycle::onCreateViews() {
    if (LifeCycle::application != nullptr) {
        LifeCycle::application->app->onCreateViews();
    }
}

void LifeCycle::onDestroyViews() {
    if (LifeCycle::application != nullptr) {
        LifeCycle::application->app->onDestroyViews();
    }
}

void LifeCycle::setApplication(const char * name, ApplicationLoader::ApplicationInstancer * appInstancer, DiligentAppBase * diligentAppBase) {
    LifeCycle::deleteApplication();
    if (appInstancer == nullptr) {
        Log::Error_And_Throw("attempting to use a nullptr application instancer");
    }
    LifeCycle::applicationInstancer = appInstancer;
    LifeCycle::application = appInstancer->createInstance(name);
    if (LifeCycle::application != nullptr) {
        LifeCycle::application->app->setDiligentAppBase(diligentAppBase);
    }
}

void LifeCycle::setApplication(ApplicationInstance *appInstance, DiligentAppBase * diligentAppBase) {
    LifeCycle::deleteApplication();
    if (appInstance == nullptr) {
        Log::Error_And_Throw("attempting to set a nullptr application instance");
    }
    LifeCycle::application = appInstance;
    if (LifeCycle::application != nullptr) {
        LifeCycle::application->app->setDiligentAppBase(diligentAppBase);
    }
}

void LifeCycle::setApplication(Application *app, DiligentAppBase * diligentAppBase) {
    LifeCycle::deleteApplication();
    if (app == nullptr) {
        Log::Error_And_Throw("attempting to set a nullptr application");
    }
    LifeCycle::application = new ApplicationInstance(app);
    if (LifeCycle::application != nullptr) {
        LifeCycle::application->app->setDiligentAppBase(diligentAppBase);
    }
}

void LifeCycle::deleteApplication() {
    LifeCycle::destroy();
    if (LifeCycle::applicationInstancer != nullptr) {
        LifeCycle::applicationInstancer->destroyInstance(LifeCycle::application);
    } else {
        if (LifeCycle::application != nullptr) {
            if (LifeCycle::application->destroyerT == nullptr) {
                delete LifeCycle::application->app;
                delete LifeCycle::application;
            } else {
                LifeCycle::application->destroyerT(LifeCycle::application);
            }
        }
    }
    LifeCycle::application = nullptr;
    LifeCycle::applicationInstancer = nullptr;
}

void LifeCycle::createState() {
    if (LifeCycle::application != nullptr) {
        if (LifeCycle::destroyed.load() && !LifeCycle::createCalled.load()) {
            LifeCycle::application->app->createState();
            LifeCycle::createCalled.store(true);
            LifeCycle::destroyed.store(false);
        }
    }
}

void LifeCycle::destroyState() {
    if (LifeCycle::application != nullptr) {
        // if we unload here then there is no way to restore the loaded
        // view unless we manually reload it with the correct type
        // which is impossible since types cannot be saved at runtime
        //
        // the view is automatically unloaded when this LifeCycle object
        // is destroyed
        if (!LifeCycle::destroyed.load() && LifeCycle::createCalled.load()) {
            LifeCycle::application->app->destroyState();
            LifeCycle::createCalled.store(false);
            LifeCycle::destroyed.store(true);
        }
    }
}

void LifeCycle::onResize(int w, int h, Diligent::ISwapChain * swapChain, Diligent::IRenderDevice * renderDevice) {
    LifeCycle::savedSwapChain = swapChain;
    LifeCycle::savedRenderDevice = renderDevice;
    LifeCycle::savedW = w;
    LifeCycle::savedH = h;
    LifeCycle::canResize.store(true);
    LifeCycle::createState();
    if (LifeCycle::application != nullptr) {
        LifeCycle::application->app->onResize(w, h, swapChain, renderDevice);
    }
}

bool LifeCycle::isDestroyed() {
    return LifeCycle::destroyed.load();
}

void LifeCycle::onDraw(Diligent::IDeviceContext *deviceContext) {
    if (LifeCycle::application != nullptr) {
        LifeCycle::application->app->onDraw(deviceContext);
    }
}

bool LifeCycle::onTouchEvent(MultiTouch &touch) {
    if (LifeCycle::application != nullptr) {
        return LifeCycle::application->app->onTouchEvent(touch);
    }
    return false;
}

void LifeCycle::create() {
    LifeCycle::onCreateViews();
    if (LifeCycle::canResize.load() && LifeCycle::application != nullptr) {
        LifeCycle::createState();
        LifeCycle::application->app->onResize(
                                         LifeCycle::savedW,
                                         LifeCycle::savedH,
                                         LifeCycle::savedSwapChain,
                                         LifeCycle::savedRenderDevice
        );
    }
}

void LifeCycle::destroy() {
    LifeCycle::destroyState();
    LifeCycle::onDestroyViews();
}

LifeCycle::~LifeCycle() {
    LifeCycle::deleteApplication();
}
