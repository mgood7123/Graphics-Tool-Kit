//
// Created by Matthew Good on 29/6/21.
//

#include "AppInstance.h"

AppInstance::AppInstance ()
{
    createCalled.store(false);
    destroyed.store(true);
    
//     800 microseconds
//    timeEngine.physicsTimeStep = 0.000800;

//     8 milliseconds
//    timeEngine.physicsTimeStep = 0.008;
    
    // 60.000024 fps, a bit choppy
//    timeEngine.physicsTimeStep = 0.01666666;

    // 120.000048 fps, smooth
    timeEngine.physicsTimeStep = 0.01666666/2;

//    timeEngine.physicsTimeStep = 0.01666666/4;
//    timeEngine.physicsTimeStep = 0.01666666/8;
//    timeEngine.physicsTimeStep = 0.01666666/16;
    
    timeEngine.physicsCallback = [&](const TimeEngine & timeEngine_) {
        ObjectBase * obj = objectBase.get<ObjectBase*>();
        if (obj != nullptr) {
            obj->physics(timeEngine_);
        }
    };

    loadObject<CubeOnTriangle>();
}

AppInstance::~AppInstance ()
{
    destroyResources();
}

void AppInstance::callCreate() {
    if (!destroyed.load() && !createCalled.load()) {
        ObjectBase * obj = objectBase.get<ObjectBase*>();
        if (obj != nullptr) {
            screenRenderTarget.create(PIPELINE_KEY, pipelineManager, m_pSwapChain, m_pDevice);
            obj->createPipeline(pipelineManager);
            obj->create();
            if (obj->hasPhysics()) {
                timeEngine.startPhysicsThread();
            }
            createCalled.store(true);
        }
    }
}

void AppInstance::callDestroy() {
    if (createCalled.load()) {
        ObjectBase *obj = objectBase.get<ObjectBase *>();
        if (obj != nullptr) {
            if (obj->hasPhysics()) {
                timeEngine.stopPhysicsThread();
            }
            obj->destroy();
            obj->destroyPipeline(pipelineManager);
            screenRenderTarget.destroy(pipelineManager);
            createCalled.store(false);
        }
    }
}

void AppInstance::unloadObject() {
    callDestroy();
    objectBase = AnyNullOpt;
}

void AppInstance::surfaceChanged (int w, int h)
{
    if (!m_pSwapChain)
    {
        attachToContext(w, h);
        destroyed.store(false);
    }

    // Resizing the swap chain requires back and depth-stencil buffers
    // to be unbound from the device context
    m_pImmediateContext->SetRenderTargets(0, nullptr, nullptr, Diligent::RESOURCE_STATE_TRANSITION_MODE_NONE);
    m_pSwapChain->Resize(w, h);
    width = w;
    height = h;
    callCreate();
    screenRenderTarget.resize(pipelineManager, m_pSwapChain, m_pDevice);
    ObjectBase * obj = objectBase.get<ObjectBase*>();
    if (obj != nullptr) {
        obj->resize(pipelineManager);
    }
}

void AppInstance::onDraw ()
{
    if (destroyed.load()) return;
    timeEngine.computeDelta();
    auto obj = objectBase.get<ObjectBase*>();
    if (obj != nullptr) {
        DrawTools drawTools(pipelineManager, pixelToNDC);

        obj->switchToPipeline(pipelineManager);
        obj->bindShaderResources(pipelineManager);
        obj->draw(drawTools, screenRenderTarget);

        drawTools.pixelToNDC.resize(width, height);
        auto color = m_pSwapChain->GetCurrentBackBufferRTV();
        auto depth = m_pSwapChain->GetDepthBufferDSV();
        RenderTarget::bind(color, depth, m_pImmediateContext);
        RenderTarget::clearColorAndDepth(RenderTarget::black, 1, color, depth, m_pImmediateContext);
        screenRenderTarget.draw(drawTools, obj->x, obj->y, obj->width, obj->height, m_pImmediateContext);
    }
    swapBuffers();
}

bool AppInstance::onTouchEvent(MultiTouch & touchEvent) {
    if (destroyed.load()) return false;
    ObjectBase * obj = objectBase.get<ObjectBase*>();
    return obj != nullptr && obj->onTouchEvent(touchEvent);
}

void AppInstance::swapBuffers ()
{
    AppInstancePlatformBase::swapBuffers();
}

void AppInstance::destroyResources ()
{
    if (destroyed.load()) return;
    destroyed.store(true);

    // if we unload here then there is no way to restore the loaded object
    // unless we manually reload it with the correct type
    // the object is automatically unloaded when this AppInstance object is destroyed

    callDestroy();

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
