//
// Created by Matthew Good on 29/6/21.
//

#include <MultiTouch/DiligentLog/Log.h>
#include "AppInstanceAndroidBase.h"

void AppInstanceAndroidBase::onEglSetup (JNIEnv * jenv, jobject classInstance, jstring name, jstring signature)
{
    if (!jvmManager.getJVM(jenv)) {
        Log::Error("failed to get JavaVM");
        return;
    }
    if (!jvmManager.attachJVM()) {
        Log::Error("failed to attach JVM");
        return;
    }
    jObject = jvmManager.globalRef(jenv, classInstance);
    jClass = jvmManager.globalRef(jenv, jenv->GetObjectClass(jObject));
    jboolean isCopy1, isCopy2;
    
    if (name == nullptr) {
        Log::Error("cannot use a null name (0x0)");
        return;
    }
    
    if (signature == nullptr) {
        Log::Error("cannot use a null signature (0x0)");
        return;
    }
    
    const char * n = jvmManager.jenv->GetStringUTFChars(name, &isCopy1);
    if (n == nullptr) {
        Log::Error("cannot get UTF chars from name");
        return;
    }
    
    const char * s = jvmManager.jenv->GetStringUTFChars(signature, &isCopy2);
    if (s == nullptr) {
        Log::Error("cannot get UTF chars from signature");
        jvmManager.jenv->ReleaseStringUTFChars(name, n);
        return;
    }
    
    jSwapBuffers = jvmManager.jenv->GetMethodID(jClass, n, s);
    
    jvmManager.jenv->ReleaseStringUTFChars(name, n);
    jvmManager.jenv->ReleaseStringUTFChars(name, s);
    
    if (jSwapBuffers == nullptr) {
        Log::Error(
                "cannot find method with name '", name, "', and signature '", signature, "'"
        );
    }
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
