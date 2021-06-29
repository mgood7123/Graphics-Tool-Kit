//
// Created by Matthew Good on 29/6/21.
//

#include "JVM_MANAGER.h"

JVM_MANAGER::JVM_MANAGER() {
    jvm = nullptr;
    jenv = nullptr;
    needsToDetach = false;
    jvmArgs.version = JNI_VERSION_1_6;
}

bool JVM_MANAGER::getJVM(JNIEnv *env) {
    return env->GetJavaVM(&jvm) == JNI_OK;
}

bool JVM_MANAGER::attachJVM() {
    // checks if current env needs attaching or it is already attached
    jint res = jvm->GetEnv((void**)&jenv, JNI_VERSION_1_6);
    if (res == JNI_EDETACHED) {
        // Supported but not attached yet, needs to call AttachCurrentThread
        res = jvm->AttachCurrentThread(&jenv, &jvmArgs);
        if (res == JNI_OK) {
            needsToDetach = true;
        } else {
            // Failed to attach, cancel
            return false;
        }
    } else if (JNI_OK == res) {
        // Current thread already attached, do not attach 'again' (just to save the attachedHere flag)
        // We make sure to keep attached = 0
        needsToDetach = false;
    } else {
        // JNI_EVERSION, specified version is not supported cancel this..
        return false;
    }
    return true;
}

void JVM_MANAGER::detachJVM() {
    if (needsToDetach) {
        jvm->DetachCurrentThread();
    }
}
