//
// Created by Matthew Good on 29/6/21.
//

#include "AppInstanceAndroidBase.h"
#include <Errors.hpp>

AppInstanceAndroidBase::AppInstanceAndroidBase ()
{
}

AppInstanceAndroidBase::~AppInstanceAndroidBase ()
{
}

void AppInstanceAndroidBase::onEglSetup (JNIEnv * jenv, jobject classInstance, jstring name, jstring signature)
{
    if (!jvmManager.getJVM(jenv)) {
        LOG_ERROR_MESSAGE("failed to get JavaVM");
        return;
    }
    if (!jvmManager.attachJVM()) {
        LOG_ERROR_MESSAGE("failed to attach JVM");
        return;
    }
    jObject = jvmManager.globalRef(jenv, classInstance);
    jClass = jvmManager.globalRef(jenv, jenv->GetObjectClass(jObject));
    jboolean isCopy1, isCopy2;
    
    if (name == nullptr) {
        LOG_ERROR_MESSAGE("cannot use a null name (0x0)");
        return;
    }
    
    if (signature == nullptr) {
        LOG_ERROR_MESSAGE("cannot use a null signature (0x0)");
        return;
    }
    
    const char * n = jvmManager.jenv->GetStringUTFChars(name, &isCopy1);
    if (n == nullptr) {
        LOG_ERROR_MESSAGE("cannot get UTF chars from name");
        return;
    }
    
    const char * s = jvmManager.jenv->GetStringUTFChars(signature, &isCopy2);
    if (s == nullptr) {
        LOG_ERROR_MESSAGE("cannot get UTF chars from signature");
        jvmManager.jenv->ReleaseStringUTFChars(name, n);
        return;
    }
    
    jSwapBuffers = jvmManager.jenv->GetMethodID(jClass, n, s);
    
    jvmManager.jenv->ReleaseStringUTFChars(name, n);
    jvmManager.jenv->ReleaseStringUTFChars(name, s);
    
    if (jSwapBuffers == nullptr) {
        LOG_ERROR_MESSAGE(
                "cannot find method with name '", name, "', and signature '", signature, "'"
        );
    }
}

bool AppInstanceAndroidBase::onTouchEvent (JNIEnv * jenv, jfloatArray motionEventData)
{
    motionEvent.acquire(jenv, motionEventData);
    bool r;
    int action = motionEvent.getAction(0);
    switch (action) {
        case MotionEventSerializer::MOTION_EVENT_ACTION_DOWN:
        case MotionEventSerializer::MOTION_EVENT_ACTION_UP: {
            /* On a touch screen move events aren't reported when the
               finger is moving above (of course), so remember the position
               always */
            previous = {motionEvent.getX(0), motionEvent.getY(0)};
            current = previous;
            // MouseEvent e(motionEvent, x, y, motionEvent.getButtonState());
            if (action == MotionEventSerializer::MOTION_EVENT_ACTION_DOWN) {
                // mousePressEvent(e);
            } else {
                // mouseReleaseEvent(e);
                // reset the relative position
                // if the relative position is not reset, then
                // the relative position of mouse press will be
                // relative to the last location of mouse release
                // which differs from desktop behaviour
                previous = {-1,-1};
            }
            r = true;
            break;
        }
        
        case MotionEventSerializer::MOTION_EVENT_ACTION_MOVE: {
            current = {motionEvent.getX(0), motionEvent.getY(0)};
            // MouseMoveEvent e{motionEvent, x, y, motionEvent.getButtonState(),
            //                  _previousMouseMovePosition == Vector2i{-1} ? Vector2i{} :
            //                  position - _previousMouseMovePosition};
            previous = current;
            // mouseMoveEvent(e);
            r = true;
            break;
        }
        default:
            r = false;
            break;
    }
    
    /** @todo Implement also other input events */
    
    motionEvent.release(jenv, motionEventData);
    return r;
}

void AppInstanceAndroidBase::onEglTearDown ()
{
    jvmManager.jenv->DeleteGlobalRef(jClass);
    jvmManager.jenv->DeleteGlobalRef(jObject);
    jvmManager.detachJVM();
    jSwapBuffers = nullptr;
}

void AppInstanceAndroidBase::swapBuffers ()
{
    if (jSwapBuffers != nullptr) {
        jvmManager.jenv->CallVoidMethod(jObject, jSwapBuffers);
    }
}
