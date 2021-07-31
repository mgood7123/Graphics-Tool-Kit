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

    timeEngine.physicsCallback = [&](const TimeEngine & timeEngine_) {
        auto obj = objectBase.get<ObjectBase*>();
        obj->physics(timeEngine_);
    };
}

AppInstance::~AppInstance ()
{
    destroyResources();
}

void AppInstance::surfaceChanged (int w, int h)
{
    bool create = false;
    if (!m_pSwapChain)
    {
        attachToContext(w, h);
        create = true;
        destroyed.store(false);
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
    if (destroyed.load()) return;
    timeEngine.computeDelta();
    auto obj = objectBase.get<ObjectBase*>();
    obj->draw();
    swapBuffers();
}

bool AppInstance::onTouchEvent(MultiTouch & touchEvent) {
    if (destroyed.load()) return false;
    return true;
//    int pointerIndex = motionEvent.getActionIndex();
//    int action = motionEvent.getAction(pointerIndex);
//    if (objectBase.get<ObjectBase*>()->onTouchEvent(motionEvent)) return true;
//    bool r;
//    switch (action) {
//        case MotionEventSerializer::MOTION_EVENT_ACTION_DOWN:
//        case MotionEventSerializer::MOTION_EVENT_ACTION_UP: {
//            /* On a touch screen move events aren't reported when the
//               finger is moving above (of course), so remember the position
//               always */
//            previous = {motionEvent.getX(0), motionEvent.getY(0)};
//            current = previous;
//            // MouseEvent e(motionEvent, x, y, motionEvent.getButtonState());
//            if (action == MotionEventSerializer::MOTION_EVENT_ACTION_DOWN) {
//                // mousePressEvent(e);
//            } else {
//                // mouseReleaseEvent(e);
//                // reset the relative position
//                // if the relative position is not reset, then
//                // the relative position of mouse press will be
//                // relative to the last location of mouse release
//                // which differs from desktop behaviour
//                previous = {-1,-1};
//            }
//            r = true;
//            break;
//        }
//
//        case MotionEventSerializer::MOTION_EVENT_ACTION_MOVE: {
//            current = {motionEvent.getX(0), motionEvent.getY(0)};
//            // MouseMoveEvent e{motionEvent, x, y, motionEvent.getButtonState(),
//            //                  _previousMouseMovePosition == Vector2i{-1} ? Vector2i{} :
//            //                  position - _previousMouseMovePosition};
//            previous = current;
//            // mouseMoveEvent(e);
//            r = true;
//            break;
//        }
//        default:
//            r = false;
//            break;
//    }

    /** @todo Implement also other input events */
//    return r;
}

void AppInstance::swapBuffers ()
{
    AppInstancePlatformBase::swapBuffers();
}

void AppInstance::destroyResources ()
{
    if (destroyed.load()) return;
    destroyed.store(true);
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
