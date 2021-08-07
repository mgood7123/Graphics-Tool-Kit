//
// Created by Matthew Good on 10/7/21.
//

#include "TimeEngine.h"
#include <chrono>
#include <iomanip>
#include <MultiTouch/DiligentLog/Primitives/interface/Errors.hpp>

TimeEngine::TimeEngine() {
    running.store(false);

    currentDeltaTime = 0.0;
    previousDeltaTime = 0.0;
    delta = 0.0;

    startElapsedTime = 0.0;
    currentElapsedTime = 0.0;
    previousElapsedTime = 0.0;
    difference = 0.0;
    elapsed = 0.0;

    physicsTimeStep = 0.0;
    logPhysics = false;

    computeCurrentDeltaTime();
    resetTimer();
}

double TimeEngine::now() {
    auto now = std::chrono::high_resolution_clock::now().time_since_epoch();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now).count();
    return static_cast<double>(ms);
}

void TimeEngine::computeCurrentDeltaTime() {
    currentDeltaTime = now()/1000;
}

void TimeEngine::computeDelta() {
    previousDeltaTime = currentDeltaTime;
    computeCurrentDeltaTime();
    delta = currentDeltaTime - previousDeltaTime;
}

void TimeEngine::resetTimer() {
    startElapsedTime = now()/1000;
    previousElapsedTime = startElapsedTime;
    currentElapsedTime = startElapsedTime;
}

void TimeEngine::elapsedTime() {
    previousElapsedTime = currentElapsedTime;
    currentElapsedTime = now()/1000;
    elapsed = currentElapsedTime - startElapsedTime;
    difference = currentElapsedTime - previousElapsedTime;
}

void TimeEngine::startPhysicsThread() {
    if (running.load()) return;
    running.store(true);
    physicsThread = std::thread([&] {
        mainLoop();
    });
}

void TimeEngine::mainLoop() {
    TimeEngine physicsTimeEngine;
    physicsTimeEngine.resetTimer();
    while(running.load()) {
        /**
         * run loop at double to fps (8 ms instead of 16.333... ms) to keep
         * things smooth
         *
         * save power by using sleep to keep thread idle
         *
         * sleep is not recommended when precision is absolutely needed
         * due to sleep not waking up at exact specified time due to scheduler
         *
         * but it will do for the physics thread
         */
        std::this_thread::sleep_for(std::chrono::milliseconds(8));
        physicsTimeEngine.elapsedTime();
        if (logPhysics) LOG_INFO_MESSAGE("physics");
        physicsTimeEngine.delta = physicsTimeStep;
        physicsCallback(physicsTimeEngine);
        physicsTimeEngine.resetTimer();
    }
}

void TimeEngine::stopPhysicsThread() {
    if (!running.load()) return;
    running.store(false);
    physicsThread.join();
}
