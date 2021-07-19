//
// Created by Matthew Good on 29/6/21.
//

#include "AppInstance.h"
#include <RectangleView.h>
#include <Cube.h>

AppInstance::AppInstance ()
{
    objectBase.disableDebug();
    objectBase = createObject<RectangleView>(this);

    // 10000000 fps, delta will be incredibly small for each step
    timeEngine.physicsTimeStep = 0.0000001;

    // 1000000 fps
    timeEngine.physicsTimeStep = 0.000001;

    // 60.000024 fps, a bit choppy
    timeEngine.physicsTimeStep = 0.01666666;

    // 120.000048 fps, smooth
//    timeEngine.physicsTimeStep = 0.01666666/2;

//    timeEngine.physicsTimeStep = 0.01666666/4;
//    timeEngine.physicsTimeStep = 0.01666666/8;
//    timeEngine.physicsTimeStep = 0.01666666/16;

    timeEngine.physicsCallback = [&](const TimeEngine & timeEngine) {
        auto obj = objectBase.get<ObjectBase*>();
        obj->physics(timeEngine);
    };
}

AppInstance::~AppInstance ()
{
}

void AppInstance::surfaceChanged (int w, int h)
{
    bool create = false;
    if (!m_pSwapChain)
    {
        attachToContext(w, h);
        create = true;
    }

    // Resizing the swap chain requires back and depth-stencil buffers
    // to be unbound from the device context
    m_pImmediateContext->SetRenderTargets(0, nullptr, nullptr, Diligent::RESOURCE_STATE_TRANSITION_MODE_NONE);
    m_pSwapChain->Resize(w, h);

    if (create)
    {
        objectBase.get<ObjectBase*>()->create();
//        timeEngine.startPhysicsThread();
    }
}

void AppInstance::onDraw ()
{
    timeEngine.computeDelta();
    auto obj = objectBase.get<ObjectBase*>();
    obj->draw();
    swapBuffers();
}

void AppInstance::swapBuffers ()
{
    AppInstancePlatformBase::swapBuffers();
}

void AppInstance::destroyResources ()
{
//    timeEngine.stopPhysicsThread();
    objectBase.get<ObjectBase*>()->destroy();
    m_pImmediateContext->Flush();
    m_pImmediateContext.Release();
    m_pSwapChain.Release();
    m_pDevice.Release();
    m_pPSO.Release();
}

#if PLATFORM_WIN32 || PLATFORM_LINUX || PLATFORM_MACOS
#elif PLATFORM_UNIVERSAL_WINDOWS
#elif PLATFORM_ANDROID
void AppInstance::onEglSetup (JNIEnv * jenv, jobject classInstance, jstring name, jstring signature)
{
    AppInstancePlatformBase::onEglSetup(jenv, classInstance, name, signature);
}

bool AppInstance::onTouchEvent (JNIEnv * jenv, jfloatArray motionEventData)
{
    return AppInstancePlatformBase::onTouchEvent(jenv, motionEventData);
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
