//
// Created by Matthew Good on 29/6/21.
//

#ifndef GRAPHICAL_TOOL_KIT_APPINSTANCE_H
#define GRAPHICAL_TOOL_KIT_APPINSTANCE_H

#include <PlatformBase/AppInstancePlatformBase.h>
#include <Objects/Objects.h>
#include <WINAPI_KERNEL/SDK/include/AnyOpt.h>


class AppInstance : public AppInstancePlatformBase, public DiligentAppBase
{
    AnyOpt objectBase;
    TimeEngine timeEngine;
    PipelineManager pipelineManager;
    PixelToNDC pixelToNDC;
    int width;
    int height;
    static constexpr const char * PIPELINE_KEY = "AppInstance RT";
    static constexpr const char * OFFSCREEN_PIPELINE_KEY = "AppInstance ORT";
    RenderTarget screenRenderTarget;

    std::atomic_bool destroyed;
    std::atomic_bool createCalled;

public:

    AppInstance ();
    ~AppInstance ();
    
    void callCreate();

    void callDestroy();

    void loadObject(ObjectBase * obj) {
        unloadObject();
        obj->setDiligentAppBase(this);
        objectBase = AnyOpt(obj, true);
        callCreate();
    }

    template <typename T, typename std::enable_if<std::is_base_of<ObjectBase, T>::value>::type* = nullptr>
    T * loadObject() {
        unloadObject();
        T * obj = new T;
        loadObject(obj);
        return obj;
    }

    void unloadObject();

#if PLATFORM_WIN32 || PLATFORM_LINUX || PLATFORM_MACOS

#elif PLATFORM_UNIVERSAL_WINDOWS
#elif PLATFORM_ANDROID
    void onEglSetup(JNIEnv *jenv, jobject classInstance, jstring name, jstring signature) override;
    void onEglTearDown() override;
#elif PLATFORM_IOS
    #elif PLATFORM_TVOS
#else
    #error "Unknown platform"
#endif

    void surfaceChanged(int w, int h);
    void onDraw();
    bool onTouchEvent(MultiTouch & touchEvent);
    void swapBuffers() override;
    void destroyResources();
};


#endif //GRAPHICAL_TOOL_KIT_APPINSTANCE_H
