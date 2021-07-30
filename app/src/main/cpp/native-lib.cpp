#include <string>
#include "AppInstance.h"

MultiTouch multiTouch;

#if PLATFORM_ANDROID

#define APP(jlong) reinterpret_cast<AppInstance*>(jlong)
#define JLONG(ptr) reinterpret_cast<jlong>(ptr)

extern "C" JNIEXPORT jlong JNICALL
Java_smallville7123_graphical_tool_kit_DiligentEngineView_00024DiligentEngineRenderer_createNativeInstance (
        JNIEnv * env, jobject thiz
)
{
    multiTouch.setMaxSupportedTouches(10);
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

extern "C" JNIEXPORT void JNICALL
Java_smallville7123_graphical_tool_kit_DiligentEngineView_00024DiligentEngineRenderer_addTouch__JFF(
        JNIEnv * env, jobject thiz,
        jlong identifier, jfloat x, jfloat y
)
{
    multiTouch.addTouch(identifier, x, y);
}

extern "C" JNIEXPORT void JNICALL
Java_smallville7123_graphical_tool_kit_DiligentEngineView_00024DiligentEngineRenderer_addTouch__JFFF (
        JNIEnv * env, jobject thiz,
        jlong identifier, jfloat x, jfloat y, jfloat size
)
{
    multiTouch.addTouch(identifier, x, y, size);
}

extern "C" JNIEXPORT void JNICALL
Java_smallville7123_graphical_tool_kit_DiligentEngineView_00024DiligentEngineRenderer_addTouch__JFFFF (
        JNIEnv * env, jobject thiz,
        jlong identifier, jfloat x, jfloat y, jfloat size, jfloat pressure
)
{
    multiTouch.addTouch(identifier, x, y, size, pressure);
}

extern "C" JNIEXPORT void JNICALL
Java_smallville7123_graphical_tool_kit_DiligentEngineView_00024DiligentEngineRenderer_moveTouch__JFF (
        JNIEnv * env, jobject thiz,
        jlong identifier, jfloat x, jfloat y
)
{
    multiTouch.moveTouch(identifier, x, y);
}

extern "C" JNIEXPORT void JNICALL
Java_smallville7123_graphical_tool_kit_DiligentEngineView_00024DiligentEngineRenderer_moveTouch__JFFF (
        JNIEnv * env, jobject thiz,
        jlong identifier, jfloat x, jfloat y, jfloat size
)
{
    multiTouch.moveTouch(identifier, x, y, size);
}

extern "C" JNIEXPORT void JNICALL
Java_smallville7123_graphical_tool_kit_DiligentEngineView_00024DiligentEngineRenderer_moveTouch__JFFFF (
        JNIEnv * env, jobject thiz,
        jlong identifier, jfloat x, jfloat y, jfloat size, jfloat pressure
)
{
    multiTouch.moveTouch(identifier, x, y, size, pressure);
}

extern "C" JNIEXPORT void JNICALL
Java_smallville7123_graphical_tool_kit_DiligentEngineView_00024DiligentEngineRenderer_removeTouch__JFF (
        JNIEnv * env, jobject thiz,
        jlong identifier, jfloat x, jfloat y
)
{
    multiTouch.removeTouch(identifier, x, y);
}

extern "C" JNIEXPORT void JNICALL
Java_smallville7123_graphical_tool_kit_DiligentEngineView_00024DiligentEngineRenderer_removeTouch__JFFF (
        JNIEnv * env, jobject thiz,
        jlong identifier, jfloat x, jfloat y, jfloat size
)
{
    multiTouch.removeTouch(identifier, x, y, size);
}

extern "C" JNIEXPORT void JNICALL
Java_smallville7123_graphical_tool_kit_DiligentEngineView_00024DiligentEngineRenderer_removeTouch__JFFFF (
        JNIEnv * env, jobject thiz,
        jlong identifier, jfloat x, jfloat y, jfloat size, jfloat pressure
)
{
    multiTouch.removeTouch(identifier, x, y, size, pressure);
}

extern "C" JNIEXPORT void JNICALL
Java_smallville7123_graphical_tool_kit_DiligentEngineView_00024DiligentEngineRenderer_cancelTouch__JFF (
        JNIEnv * env, jobject thiz,
        jlong identifier, jfloat x, jfloat y
)
{
    multiTouch.cancelTouch(identifier, x, y);
}

extern "C" JNIEXPORT void JNICALL
Java_smallville7123_graphical_tool_kit_DiligentEngineView_00024DiligentEngineRenderer_cancelTouch__JFFF (
        JNIEnv * env, jobject thiz,
        jlong identifier, jfloat x, jfloat y, jfloat size
)
{
    multiTouch.cancelTouch(identifier, x, y, size);
}

extern "C" JNIEXPORT void JNICALL
Java_smallville7123_graphical_tool_kit_DiligentEngineView_00024DiligentEngineRenderer_cancelTouch__JFFFF (
        JNIEnv * env, jobject thiz,
        jlong identifier, jfloat x, jfloat y, jfloat size, jfloat pressure
)
{
    multiTouch.cancelTouch(identifier, x, y, size, pressure);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_smallville7123_graphical_tool_kit_DiligentEngineView_00024DiligentEngineRenderer_onTouchEvent (
        JNIEnv * env, jobject thiz, jlong instance
)
{
    return APP(instance)->onTouchEvent(multiTouch);
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

#endif
