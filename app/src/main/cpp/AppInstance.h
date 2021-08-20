//
// Created by Matthew Good on 29/6/21.
//

#ifndef GRAPHICAL_TOOL_KIT_APPINSTANCE_H
#define GRAPHICAL_TOOL_KIT_APPINSTANCE_H

#include <PlatformBase/AppInstancePlatformBase.h>
#include <Views/Views.h>
#include <WINAPI_KERNEL/SDK/include/AnyOpt.h>


class AppInstance : public AppInstancePlatformBase, public DiligentAppBase
{
    AnyOpt contents;
    TimeEngine timeEngine;
    PipelineManager pipelineManager;
    PixelToNDC pixelToNDC;
    int width;
    int height;
    static constexpr const char * PIPELINE_KEY = "AppInstance RT";
    RenderTarget screen;
    RenderTarget rt;

    std::atomic_bool destroyed;
    std::atomic_bool createCalled;

public:

    AppInstance ();
    ~AppInstance ();
    
    void callCreate();

    void callDestroy();

    void loadView(View * view) {
        unloadView();
        view->setDiligentAppBase(this);
        contents = AnyOpt(view, true);
        callCreate();
    }

    template <typename T, typename std::enable_if<std::is_base_of<View, T>::value>::type* = nullptr>
    T * loadView() {
        unloadView();
        T * obj = new T;
        loadView(obj);
        return obj;
    }

    void unloadView();

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
