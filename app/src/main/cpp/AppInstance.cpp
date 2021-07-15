//
// Created by Matthew Good on 29/6/21.
//

#include "AppInstance.h"
#include <Triangle.h>
#include <Cube.h>

AppInstance::AppInstance ()
{
    objectBase.disableDebug();
    Cube cube;
    cube.diligentAppBase = this;
    objectBase = std::move(cube);

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

void AppInstance::onEglSetup (JNIEnv * jenv, jobject classInstance, jstring name, jstring signature)
{
    AppInstanceAndroidBase::onEglSetup(jenv, classInstance, name, signature);
}

void AppInstance::surfaceChanged (int w, int h)
{
    if (m_pSwapChain)
    {
        m_pSwapChain->Resize(w, h);
    }
    else
    {
        attachToContext(w, h);
        m_pSwapChain->Resize(w, h);
        objectBase.get<ObjectBase*>()->create();
        timeEngine.startPhysicsThread();
    }
}

bool AppInstance::onTouchEvent (JNIEnv * jenv, jfloatArray motionEventData)
{
    return AppInstanceAndroidBase::onTouchEvent(jenv, motionEventData);
}

void AppInstance::onDraw ()
{
    timeEngine.computeDelta();
    auto obj = objectBase.get<ObjectBase*>();
    obj->draw();
    swapBuffers();
}

void AppInstance::onEglTearDown ()
{
    timeEngine.stopPhysicsThread();
    objectBase.get<ObjectBase*>()->destroy();
    m_pImmediateContext->Flush();
    m_pImmediateContext.Release();
    m_pSwapChain.Release();
    m_pDevice.Release();
    m_pPSO.Release();
    AppInstanceAndroidBase::onEglTearDown();
}

void AppInstance::swapBuffers ()
{
    AppInstanceAndroidBase::swapBuffers();
}
