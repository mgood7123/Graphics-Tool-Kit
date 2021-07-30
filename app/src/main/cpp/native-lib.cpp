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
Java_smallville7123_graphical_tool_kit_DiligentEngineView_00024DiligentEngineRenderer_removeTouch__JJFF (
        JNIEnv * env, jobject thiz, jlong instance,
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

#elif PLATFORM_MACOS

AppInstance * instance;

/* A simple function that prints a message, the error code returned by SDL,
 * and quits the application */
void sdldie(const char *msg)
{
    LOG_ERROR_MESSAGE(msg, ": ", SDL_GetError());
    LOG_ERROR_MESSAGE("DESTROY APP"); delete instance;
    SDL_Quit();
    exit(1);
}


void checkSDLError(int line = -1)
{
#ifndef NDEBUG
	const char *error = SDL_GetError();
	if (*error != '\0')
	{
		LOG_ERROR_MESSAGE("SDL Error: ", error);
		if (line != -1)
			LOG_ERROR_MESSAGE(" + line: ", line);
		SDL_ClearError();
	}
#endif
}

// this may be called from another thread
int filterEvents(void *userdata, SDL_Event *ev) {
    SDL_Event & event = *ev;
    switch( event.type ) {
        case SDL_WINDOWEVENT:
            switch ( event.window.event ) {
                case SDL_WINDOWEVENT_RESIZED:
                    instance->surfaceChanged(event.window.data1, event.window.data2);
                    instance->onDraw();
                    return 1;
            }
            break;
    }
    return 1;
}

/* Our program's entry point */
int main(int argc, char *argv[])
{
    LOG_ERROR_MESSAGE("CREATE APP"); instance = new AppInstance();

    SDL_GLContext maincontext; /* Our opengl context handle */

    if (SDL_Init(SDL_INIT_VIDEO) < 0) /* Initialize SDL's Video subsystem */
        sdldie("Unable to initialize SDL"); /* Or die on error */

    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    /* Turn on double buffering with a 24bit Z buffer.
     * You may need to change this to 16 or 32 for your system */
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    /* Create our window centered at 512x512 resolution */
    instance->mainwindow = SDL_CreateWindow(
            "SDL2 Application",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 512, 512,
                SDL_WINDOW_OPENGL
                | SDL_WINDOW_SHOWN
                | SDL_WINDOW_RESIZABLE
    );

    if (!instance->mainwindow) /* Die if creation failed */
        sdldie("Unable to create window");

    checkSDLError(__LINE__);

    /* Create our opengl context and attach it to our window */
    maincontext = SDL_GL_CreateContext(instance->mainwindow);
    checkSDLError(__LINE__);


    /* This makes our buffer swap syncronized with the monitor's vertical refresh */
    SDL_GL_SetSwapInterval(1);

    instance->surfaceChanged(512, 512);

    bool running = true;

    SDL_SetEventFilter((SDL_EventFilter)filterEvents, nullptr);

    /*
     * Now we want to begin our normal app process--
     * an event loop with a lot of redrawing.
     */
    while(running) {
        /* Process incoming events. */

        /* Our SDL event placeholder. */
        SDL_Event event;

        /* Grab all the events off the queue. */
        while( SDL_PollEvent( &event ) ) {
            switch( event.type ) {
                case SDL_KEYDOWN:
                    /* Handle key presses. */
                    /*
                     * We're only interested if 'Esc' has
                     * been presssed.
                     *
                     * EXERCISE:
                     * Handle the arrow keys and have that change the
                     * viewing position/angle.
                     */
                    switch( event.key.keysym.sym ) {
                        case SDLK_ESCAPE:
                            running = false;
                            break;
                        case SDLK_SPACE:
                            break;
                        default:
                            break;
                    }
                    break;
                case SDL_QUIT:
                    /* Handle quit requests (like Ctrl-c). */
                    running = false;
                    break;
            }
        }
        instance->onDraw();
    }


    /* Delete our opengl context, destroy our window, and shutdown SDL */
    SDL_GL_DeleteContext(maincontext);
    instance->destroyResources();
    SDL_DestroyWindow(instance->mainwindow);
    LOG_ERROR_MESSAGE("DESTROY APP"); delete instance;

    SDL_Quit();

    return 0;
}

#endif