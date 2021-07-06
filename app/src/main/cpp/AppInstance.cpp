//
// Created by Matthew Good on 29/6/21.
//

#include "AppInstance.h"
#include <Triangle.h>
#include <Cube.h>

AppInstance::AppInstance ()
{
    objectBase.enableDebug();
    Cube cube;
    cube.diligentAppBase = this;
    objectBase = std::move(cube);
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
        objectBase.get<ObjectBase*>()->create();
    }
}

bool AppInstance::onTouchEvent (JNIEnv * jenv, jfloatArray motionEventData)
{
    return AppInstanceAndroidBase::onTouchEvent(jenv, motionEventData);
}

void AppInstance::onDraw ()
{
    objectBase.disableDebug();
    objectBase.get<ObjectBase*>()->draw();
    objectBase.enableDebug();
    swapBuffers();
}

void AppInstance::onEglTearDown ()
{
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
