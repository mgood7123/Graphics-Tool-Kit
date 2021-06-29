#include <string>
#include "../AppInstance.h"

#define APP(jlong) reinterpret_cast<AppInstance*>(jlong)
#define JLONG(ptr) reinterpret_cast<jlong>(ptr)

extern "C" JNIEXPORT jlong JNICALL
Java_smallville7123_graphical_tool_kit_DiligentEngineView_00024DiligentEngineRenderer_createNativeInstance (
        JNIEnv * env, jobject thiz
)
{
    LOG_ERROR_MESSAGE("CREATE APP");
    return JLONG(new AppInstance());
}
extern "C" JNIEXPORT void JNICALL
Java_smallville7123_graphical_tool_kit_DiligentEngineView_00024DiligentEngineRenderer_destroyNativeInstance (
        JNIEnv * env, jobject thiz, jlong instance
)
{
    LOG_ERROR_MESSAGE("DESTROY APP");
    delete APP(instance);
}
extern "C" JNIEXPORT void JNICALL
Java_smallville7123_graphical_tool_kit_DiligentEngineView_00024DiligentEngineRenderer_onEglSetup (
        JNIEnv * env, jobject thiz, jlong instance, jobject class_instance, jstring name,
        jstring signature
)
{
    LOG_ERROR_MESSAGE("EGL SETUP");
    APP(instance)->onEglSetup(env, class_instance, name, signature);
}
extern "C" JNIEXPORT void JNICALL
Java_smallville7123_graphical_tool_kit_DiligentEngineView_00024DiligentEngineRenderer_surfaceChanged (
        JNIEnv * env, jobject thiz, jlong instance, jint w, jint h
)
{
    LOG_ERROR_MESSAGE("EGL SURFACE CHANGE");
    APP(instance)->surfaceChanged(w, h);
}
extern "C" JNIEXPORT jboolean JNICALL
Java_smallville7123_graphical_tool_kit_DiligentEngineView_00024DiligentEngineRenderer_onTouchEvent (
        JNIEnv * env, jobject thiz, jlong instance, jfloatArray motion_event
)
{
    return APP(instance)->onTouchEvent(env, motion_event);
}
extern "C" JNIEXPORT void JNICALL
Java_smallville7123_graphical_tool_kit_DiligentEngineView_00024DiligentEngineRenderer_onDraw (
        JNIEnv * env, jobject thiz, jlong instance
)
{
    APP(instance)->onDraw();
}
extern "C" JNIEXPORT void JNICALL
Java_smallville7123_graphical_tool_kit_DiligentEngineView_00024DiligentEngineRenderer_onEglTearDown (
        JNIEnv * env, jobject thiz, jlong instance
)
{
    LOG_ERROR_MESSAGE("EGL TEAR DOWN");
    APP(instance)->onEglTearDown();
}