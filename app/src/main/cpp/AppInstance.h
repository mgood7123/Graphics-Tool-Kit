//
// Created by Matthew Good on 29/6/21.
//

#ifndef GRAPHICAL_TOOL_KIT_APPINSTANCE_H
#define GRAPHICAL_TOOL_KIT_APPINSTANCE_H

#include <AppInstancePlatformBase.h>
#include <ObjectBase.h>
#include <SDK/include/AnyOpt.h>


class AppInstance : public AppInstancePlatformBase, public DiligentAppBase
{
    AnyOpt objectBase;
    TimeEngine timeEngine;
    
    std::atomic_bool destroyed;

public:

    AppInstance ();
    ~AppInstance ();

    template <typename Object>
    AnyOpt createObject(DiligentAppBase * diligentAppBase) {
        Object * obj = new Object;
        obj->diligentAppBase = diligentAppBase;
        return AnyOpt(obj, true);
    }

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
    void swapBuffers() override;
    void destroyResources();
};


#endif //GRAPHICAL_TOOL_KIT_APPINSTANCE_H
