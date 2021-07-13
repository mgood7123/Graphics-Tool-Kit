//
// Created by Matthew Good on 10/7/21.
//

#ifndef GRAPHICAL_TOOL_KIT_TIMEENGINE_H
#define GRAPHICAL_TOOL_KIT_TIMEENGINE_H

#include <functional>
#include <thread>
#include <atomic>

class TimeEngine {
    std::atomic_bool running;
    std::thread physicsThread;
public:
    double currentDeltaTime;
    double previousDeltaTime;
    double delta;

    double startElapsedTime;
    double currentElapsedTime;
    double previousElapsedTime;
    double difference;
    double elapsed;

    // 1000/60 is 16.6666666667 ms
    // 1000/16.6666666667 is 59.9999999999 fps
    //
    // 1/60 is 0.01666666666 ms in seconds
    // 1/0.01666666666 is 60.000000024 fps
    //
    //
    // a physics thread will ALWAYS be slower than a draw thread
    // since the physics call IS NOT guaranteed to be called
    // every single frame due to thread scheduling
    double physicsTimeStep;

    bool logPhysics;

    TimeEngine();

    double now();
    void computeCurrentDeltaTime();
    void computeDelta();

    void resetTimer();
    void elapsedTime();

    // a physics thread will ALWAYS be slower than a draw thread
    // since the physics call IS NOT guaranteed to be called
    // every single frame due to thread scheduling
    void startPhysicsThread();

    // a physics thread will ALWAYS be slower than a draw thread
    // since the physics call IS NOT guaranteed to be called
    // every single frame due to thread scheduling
    void stopPhysicsThread();

    // a physics thread will ALWAYS be slower than a draw thread
    // since the physics call IS NOT guaranteed to be called
    // every single frame due to thread scheduling
    std::function<void(const TimeEngine &)> physicsCallback;
};


#endif //GRAPHICAL_TOOL_KIT_TIMEENGINE_H
