//
// Created by Matthew Good on 29/6/21.
//

#ifndef GRAPHICAL_TOOL_KIT_APPINSTANCE_H
#define GRAPHICAL_TOOL_KIT_APPINSTANCE_H

#include <AppInstanceAndroidBase.h>
#include <DiligentAppBase.h>

class AppInstance : public AppInstanceAndroidBase, public DiligentAppBase
{
public:
    AppInstance ();
    ~AppInstance ();
    
    void onEglSetup(JNIEnv *jenv, jobject classInstance, jstring name, jstring signature) override;
    void surfaceChanged(int w, int h);
    bool onTouchEvent(JNIEnv *jenv, jfloatArray motionEventData) override;
    void onDraw();
    void onEglTearDown() override;
    void swapBuffers() override;
};


#endif //GRAPHICAL_TOOL_KIT_APPINSTANCE_H
