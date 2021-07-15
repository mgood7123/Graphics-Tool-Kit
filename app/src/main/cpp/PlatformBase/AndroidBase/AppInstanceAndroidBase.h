//
// Created by Matthew Good on 29/6/21.
//

#ifndef GRAPHICAL_TOOL_KIT_APPINSTANCEANDROIDBASE_H
#define GRAPHICAL_TOOL_KIT_APPINSTANCEANDROIDBASE_H

#include "JVM_MANAGER.h"
#include "MotionEventSerializer.h"

class AppInstanceAndroidBase
{
public:
    struct MousePos {
        float x;
        float y;
    };
    
    JVM_MANAGER jvmManager;
    jobject jObject;
    jclass jClass;
    jmethodID jSwapBuffers;
    MotionEventSerializer motionEvent;
    
    MousePos previous;
    MousePos current;
    
    // delete called on non-final that has virtual functions but non-virtual destructor
    virtual ~AppInstanceAndroidBase() = default;
    virtual void swapBuffers();

    virtual void onEglSetup(JNIEnv *jenv, jobject classInstance, jstring name, jstring signature);
    virtual bool onTouchEvent(JNIEnv *jenv, jfloatArray motionEventData);
    virtual void onEglTearDown();
};


#endif //GRAPHICAL_TOOL_KIT_APPINSTANCEANDROIDBASE_H
