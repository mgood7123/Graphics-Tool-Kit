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

    template <typename Object, typename std::enable_if<std::is_base_of<ObjectBase, Object>::value>::type* = nullptr>
    AnyOpt createObject(DiligentAppBase * diligentAppBase) {
        Object * obj = new Object;
        dynamic_cast<ObjectBase*>(obj)->setDiligentAppBase(diligentAppBase);
        return AnyOpt(obj, true);
    }

public:

    AppInstance ();
    ~AppInstance ();
    
    void callCreate();

    void callDestroy();

    template <typename T>
    void loadObject() {
        callDestroy();
        objectBase = createObject<T>(this);
        callCreate();
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
