//
// Created by Matthew Good on 29/6/21.
//

#ifndef GRAPHICAL_TOOL_KIT_APPINSTANCE_H
#define GRAPHICAL_TOOL_KIT_APPINSTANCE_H

#include <PlatformBase/AppInstancePlatformBase.h>
#include <Application/LifeCycle.h>
#include <Application/ApplicationLoader.h>
#include <Applications/Applications.h>

class AppInstance : public AppInstancePlatformBase, public DiligentAppBase
{
    LifeCycle life;
    ApplicationLoader loader;
public:

    AppInstance();
    ~AppInstance();

    void loadApplication(const char * name, ApplicationLoader::ApplicationInstancer * appInstancer);
    void loadApplication(ApplicationInstance * appInstance);
    void loadApplication(Application * app);
    void loadApplication(View *view);

    void unloadApplication();

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
