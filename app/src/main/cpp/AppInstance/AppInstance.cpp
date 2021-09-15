//
// Created by Matthew Good on 29/6/21.
//

#include "AppInstance.h"

AppInstance::AppInstance() {
//    loadApplication(new TriangleView());
//    loadApplication(new TextViewTest());
//    loadApplication(new GridViewTest());
    loadApplication(new LinearLayoutTest());
//    DebugView * debugView = new DebugView();
//    debugView->addView(new CubeView());
//    loadApplication(new Application(debugView));
}

AppInstance::~AppInstance ()
{
    destroyResources();

    // life muse be deleted before ApplicationLoader's destructor is called
    unloadApplication();
}

void AppInstance::loadApplication(const char * name, ApplicationLoader::ApplicationInstancer * appInstancer) {
    life.setApplication(name, appInstancer, this);
    life.create();
}

void AppInstance::loadApplication(ApplicationInstance *appInstance) {
    life.setApplication(appInstance, this);
    life.create();
}

void AppInstance::loadApplication(Application *app) {
    life.setApplication(app, this);
    life.create();
}

void AppInstance::loadApplication(View *view) {
    loadApplication(new Application(view));
}


void AppInstance::unloadApplication() {
    life.deleteApplication();
}

void AppInstance::surfaceChanged (int w, int h)
{
    if (!m_pSwapChain) {
        attachToContext(w, h);
    }

    // Resizing the swap chain requires back and depth-stencil buffers
    // to be unbound from the device context
    m_pImmediateContext->SetRenderTargets(0, nullptr, nullptr, Diligent::RESOURCE_STATE_TRANSITION_MODE_NONE);
    m_pSwapChain->Resize(w, h);
    swapChainWidth = w;
    swapChainHeight = h;
    life.onResize(w, h, m_pSwapChain, m_pDevice);
}

void AppInstance::onDraw ()
{
    if (life.isDestroyed()) {
        // don't swapping buffers if application is in destroyed state
        return;
    }
    life.onMeasure();
    life.onLayout();
    life.onDraw(m_pImmediateContext);
    swapBuffers();
}

bool AppInstance::onTouchEvent(MultiTouch & touchEvent) {
    return life.onTouchEvent(touchEvent);
}

void AppInstance::swapBuffers ()
{
    AppInstancePlatformBase::swapBuffers();
}

void AppInstance::destroyResources ()
{
    if (life.isDestroyed()) return;
    life.destroy();

    m_pImmediateContext->Flush();
    m_pImmediateContext.Release();
    m_pSwapChain.Release();
    m_pDevice.Release();
}

#if PLATFORM_WIN32 || PLATFORM_LINUX || PLATFORM_MACOS
#elif PLATFORM_UNIVERSAL_WINDOWS
#elif PLATFORM_ANDROID
void AppInstance::onEglSetup (JNIEnv * jenv, jobject classInstance, jstring name, jstring signature)
{
    AppInstancePlatformBase::onEglSetup(jenv, classInstance, name, signature);
}

void AppInstance::onEglTearDown ()
{
    destroyResources();
    AppInstancePlatformBase::onEglTearDown();
}

#elif PLATFORM_IOS
#elif PLATFORM_TVOS
#else
    #error "Unknown platform"
#endif
