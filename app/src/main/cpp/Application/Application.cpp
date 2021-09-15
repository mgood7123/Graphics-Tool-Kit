//
// Created by Matthew Good on 25/8/21.
//

#include "Application.h"

Application::Application() {
    // 120.000048 fps, smooth
    timeEngine.physicsTimeStep = 0.01666666/2;
    timeEngine.physicsCallback = [&](const TimeEngine & timeEngine_) {
        if (contentView != nullptr) {
            contentView->physics(timeEngine_);
        }
    };
}

Application::Application(View * content): Application() {
    setContentView(content);
}

void Application::createState() {
    // protect against createState being called multiple times
    if (destroyed.load() && !createCalled.load()) {
        if (contentView != nullptr) {
            contentView->createPipeline(pipelineManager);
            contentView->create();
            if (contentView->hasPhysics()) {
                timeEngine.startPhysicsThread();
            }
        }
        createCalled.store(true);
        destroyed.store(false);
    }
}

void Application::destroyState() {
    // protect against destroyState being called multiple times
    if (!destroyed.load() && createCalled.load()) {
        if (contentView != nullptr) {
            if (contentView->hasPhysics()) {
                timeEngine.stopPhysicsThread();
            }
            contentView->destroy();
            contentView->destroyPipeline(pipelineManager);
        }
        if (rt.exists()) rt.destroy(pipelineManager);
        createCalled.store(false);
        destroyed.store(true);
    }
}

Application::~Application() {
    onDestroyViews();
    destroyState();
    delete contentView;
}

void Application::setDiligentAppBase(DiligentAppBase *base) {
    diligentAppBase = base;
    if (contentView != nullptr) {
        contentView->setDiligentAppBase(diligentAppBase);
    }
}

void Application::onCreateViews() {

}

void Application::onDestroyViews() {
}

void Application::onResize(int w, int h, Diligent::ISwapChain * swapChain, Diligent::IRenderDevice * renderDevice) {
    width = w;
    height = h;
    screen.wrap(
            swapChain->GetCurrentBackBufferRTV(),
            swapChain->GetDepthBufferDSV()
    );
    screen.resize(pipelineManager, swapChain, renderDevice);
    if (!rt.exists()) {
        rt.create(PIPELINE_KEY, pipelineManager, swapChain, renderDevice);
    }
    rt.resize(pipelineManager, swapChain, renderDevice);
    if (contentView != nullptr) {
        contentView->resize(pipelineManager);
    }
}

void Application::onMeasure() {
    if (contentView != nullptr) {
        contentView->measure();
    }
}

void Application::onLayout() {
    if (contentView != nullptr) {
        Position dim = contentView->getMeasuredDimensions();

        float w = View::minmax(dim.x, 100);
        float h = View::minmax(dim.y, 100);

        DrawTools drawTools(pipelineManager, pixelToNDC);
        drawTools.pixelToNDC.resize(100, 100);
        contentView->buildCoordinates({0, 0, w, h}, drawTools, rt);
        contentView->layout(contentView->getRelativePosition(), drawTools, screen, rt);
    }
}

void Application::onDraw(Diligent::IDeviceContext *deviceContext) {
    timeEngine.computeDelta();

    // clear screen
    screen.bind(deviceContext);
    screen.clearColorAndDepth(RenderTarget::black, 1, deviceContext);

    if (contentView != nullptr) {
        DrawTools drawTools(pipelineManager, pixelToNDC);
        contentView->switchToPipeline(pipelineManager);
        contentView->bindShaderResources(pipelineManager);

        // clear render target
        rt.bind(deviceContext);
        rt.clearColorAndDepth(RenderTarget::black, 1, deviceContext);

        // draw view to render target, screen is unused
        // each ViewGroup draws its RT to the argument `rt`
        contentView->draw(drawTools, screen, rt);

        screen.bind(deviceContext);
        
        Position drawDimensions = contentView->getDrawDimensions();
        Rectangle drawPosition = contentView->getDrawPosition();

        if (contentView->printLogging) {
            Log::Info("TAG: ", "APPLICATION -> ", contentView->getTag(), ", resizing pixel grid to             : ", drawDimensions);
        }
        drawTools.pixelToNDC.resize(drawDimensions.x, drawDimensions.y);
        rt.clip(deviceContext);
        if (contentView->printLogging) {
            Log::Info("TAG: ", "APPLICATION -> ", contentView->getTag(), ", drawing absolute position          : ", drawPosition);
        }
        rt.drawAbsolutePosition(drawTools, drawPosition, deviceContext);
    }
}

bool Application::onTouchEvent(MultiTouch &touch) {
    if (contentView != nullptr) {
        return contentView->onTouchEvent(touch);
    }
    return false;
}

void Application::setContentView(View *view) {
    onDestroyViews();
    if (contentView != nullptr) {
        if (contentView->hasPhysics()) {
            timeEngine.stopPhysicsThread();
        }
        contentView->destroy();
        contentView->destroyPipeline(pipelineManager);
    }
    delete contentView;
    if (view != nullptr) {
        if (View::isInstanceOf<ViewGroup>(view)) {
            contentView = view;
        } else {
            Log::Info("wrapping View inside of a RootView");
            // wrap the view inside of view group
            auto * r = new RootView();
            r->addView(view);
            contentView = r;
        }
    }
    if (contentView != nullptr) {
        contentView->setDiligentAppBase(diligentAppBase);
    }
}

ApplicationInstance::ApplicationInstance(Application *application) {
    app = application;
}
