//
// Created by Matthew Good on 29/6/21.
//

#ifndef GRAPHICAL_TOOL_KIT_JVM_MANAGER_H
#define GRAPHICAL_TOOL_KIT_JVM_MANAGER_H
#include <jni.h>

class JVM_MANAGER {
public:
    JavaVM * jvm;
    JNIEnv* jenv;
    JavaVMAttachArgs jvmArgs;
    bool needsToDetach;
    
    JVM_MANAGER();
    bool getJVM(JNIEnv * env);
    bool attachJVM();
    void detachJVM();
    
    template <typename J, typename O> O globalRef(J j, O o) {
        return reinterpret_cast<O>(j->NewGlobalRef(o));
    }
};

#endif //GRAPHICAL_TOOL_KIT_JVM_MANAGER_H
