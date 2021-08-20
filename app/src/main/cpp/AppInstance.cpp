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
        View * view = contents.get<View*>();
        if (view != nullptr) {
            view->physics(timeEngine_);
        }
    };
}

AppInstance::~AppInstance ()
{
    destroyResources();
}

void AppInstance::callCreate() {
    if (!destroyed.load() && !createCalled.load()) {
        // create even if we dont have a view
        rt.create(PIPELINE_KEY, pipelineManager, m_pSwapChain, m_pDevice);
        View * view = contents.get<View*>();
        if (view != nullptr) {
            view->createPipeline(pipelineManager);
            view->create();
            if (view->hasPhysics()) {
                timeEngine.startPhysicsThread();
            }
        }
        createCalled.store(true);
    }
}

void AppInstance::callDestroy() {
    if (createCalled.load()) {
        View *view = contents.get<View *>();
        if (view != nullptr) {
            if (view->hasPhysics()) {
                timeEngine.stopPhysicsThread();
            }
            view->destroy();
            view->destroyPipeline(pipelineManager);
        }
        rt.destroy(pipelineManager);
        createCalled.store(false);
    }
}

void AppInstance::unloadView() {
    callDestroy();
    contents = AnyNullOpt;
}

RootView * a;
RootView * b;
GridView * c;
TouchDetectorPainter * t;

void AppInstance::surfaceChanged (int w, int h)
{
    Log::Info("root window: w: ", w, ", h: ", h);
    if (!m_pSwapChain)
    {
        attachToContext(w, h);
        destroyed.store(false);
        
        a = new RootView();
        a->setTag("root");
        a->padding = 25;
        loadView(a);
        
        b = new RootView();
        b->setTag("root child");
//        b->padding = 25;
        a->addView(b);
        
        c = new GridView();
        c->setTag("grid");
        c->addView(new TouchDetectorPainter());
        auto * x = new TouchDetectorPainter();
        x->padding = 50;
        c->addView(x);
        c->addView(new TouchDetectorPainter());
        c->addView(new TouchDetectorPainter());
        b->addView(c);
    }

    // Resizing the swap chain requires back and depth-stencil buffers
    // to be unbound from the device context
    m_pImmediateContext->SetRenderTargets(0, nullptr, nullptr, Diligent::RESOURCE_STATE_TRANSITION_MODE_NONE);
    m_pSwapChain->Resize(w, h);
    width = w;
    height = h;
    swapChainWidth = w;
    swapChainHeight = h;
    callCreate();
    screen.wrap(
        m_pSwapChain->GetCurrentBackBufferRTV(),
        m_pSwapChain->GetDepthBufferDSV()
    );
    screen.resize(pipelineManager, m_pSwapChain, m_pDevice);
    rt.resize(pipelineManager, m_pSwapChain, m_pDevice);
    View * view = contents.get<View*>();
    if (view != nullptr) {
        view->resize(pipelineManager);
    }
}

void AppInstance::onDraw ()
{
    if (destroyed.load()) return;
    timeEngine.computeDelta();

    // clear screen
    screen.bind(m_pImmediateContext);
    screen.clearColorAndDepth(RenderTarget::black, 1, m_pImmediateContext);

    auto view = contents.get<View*>();
    if (view != nullptr) {
        DrawTools drawTools(pipelineManager, pixelToNDC);
        Rectangle drawPosition = {0, 0, 100, 100};

        view->switchToPipeline(pipelineManager);
        view->bindShaderResources(pipelineManager);

        drawTools.pixelToNDC.resize(100, 100);

        view->buildCoordinates(drawPosition, drawTools, rt);

        // clear render target
        rt.bind(m_pImmediateContext);
        rt.clearColorAndDepth(RenderTarget::black, 1, m_pImmediateContext);

        // draw view to render target, screen is unused
        // each ViewGroup draws its RT to the argument `rt`
        // rt.drawToRenderTarget(drawTools, renderTarget, app.m_pImmediateContext);
        view->draw(drawTools, screen, rt);

        // draw render target to screen
        rt.drawToRenderTarget(drawTools, screen, m_pImmediateContext);
    }
    swapBuffers();
}

bool AppInstance::onTouchEvent(MultiTouch & touchEvent) {
    if (destroyed.load()) return false;
    View * view = contents.get<View*>();
    if (view != nullptr) {
        return view->onTouchEvent(touchEvent);
    }
    return false;
}

void AppInstance::swapBuffers ()
{
    AppInstancePlatformBase::swapBuffers();
}

void AppInstance::destroyResources ()
{
    if (destroyed.load()) return;
    destroyed.store(true);

    // if we unload here then there is no way to restore the loaded
    // view unless we manually reload it with the correct type
    // which is impossible since types cannot be saved at runtime
    //
    // the view is automatically unloaded when this AppInstance
    // object is destroyed

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
