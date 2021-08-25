//
// Created by Matthew Good on 31/7/21.
//

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
    mes.setMaxSupportedPointers(10);
    motionEventSerializer.setMaxSupportedPointers(10);

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

    long pointer_index;

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
                case SDL_MOUSEBUTTONDOWN:
                    mes.addPointer();
                    mes.setAction(MotionEventSynthesizer::MotionEvent::ACTION_POINTER_DOWN);
                    pointer_index = mes.getCurrentPointer();
                    mes.setData(pointer_index, 0, 0, 0, 0);
                    motionEvent = mes.produceMotionEvent();

                    motionEventSerializer.acquire(motionEvent);
                    instance->onTouchEvent(motionEventSerializer);
                    motionEventSerializer.release(motionEvent);

                    break;
                case SDL_MOUSEBUTTONUP:
                    pointer_index = mes.getCurrentPointer();
                    mes.setData(pointer_index, 0, 0, 0, 0);
                    mes.setAction(
                            MotionEventSynthesizer::MotionEvent::ACTION_POINTER_UP
                    );
                    motionEvent = mes.produceMotionEvent();

                    motionEventSerializer.acquire(motionEvent);
                    instance->onTouchEvent(motionEventSerializer);
                    motionEventSerializer.release(motionEvent);

                    mes.removePointer();
                    pointer_index = mes.getCurrentPointer();
                    break;
                case SDL_MOUSEMOTION:
                    mes.setAction(
                            MotionEventSynthesizer::MotionEvent::ACTION_MOVE
                    );
                    motionEvent = mes.produceMotionEvent();

                    motionEventSerializer.acquire(motionEvent);
                    instance->onTouchEvent(motionEventSerializer);
                    motionEventSerializer.release(motionEvent);

                    break;
                case SDL_KEYDOWN:
                    /* Handle key presses. */
                    switch( event.key.keysym.sym ) {
                        case SDLK_ESCAPE:
                            running = false;
                            break;
                        default:
                            break;
                    }
                    break;
                case SDL_QUIT:
                    /* Handle quit requests (like Ctrl-c). */
                    running = false;
                    break;
                default:
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
