//
// Created by Matthew Good on 10/7/21.
//

#include "TimeEngine.h"
#include <chrono>
#include <iomanip>
#include <DiligentCore/Primitives/interface/Errors.hpp>

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
        TimeEngine physicsTimeEngine;
        physicsTimeEngine.resetTimer();
        uint64_t skipped = 0;
        while(running.load()) {
            physicsTimeEngine.elapsedTime();
            if (physicsTimeEngine.previousElapsedTime == physicsTimeEngine.currentElapsedTime) {
                skipped++;
                continue;
            }
            if (skipped > 0) {
                LOG_INFO_MESSAGE("physics");
                if (logPhysics) LOG_INFO_MESSAGE("skipped ", skipped, " empty frame", skipped == 1 ? "" : "s");
                skipped = 0;
            }
            if (physicsTimeEngine.elapsed == physicsTimeStep) {
                physicsTimeEngine.delta = physicsTimeStep;
                physicsCallback(physicsTimeEngine);
                physicsTimeEngine.resetTimer();
            } else if (physicsTimeEngine.elapsed > physicsTimeStep) {
                /**
                 * catch up:
                 *
                 * if an object suddenly teleports half way across the map due
                 * to 8 seconds of delta, then the collision checking code will
                 * not work or completely break
                 */
                physicsTimeEngine.delta = physicsTimeStep;
                double remaining = physicsTimeEngine.elapsed;
                int catch_up = 0;
                while (remaining > physicsTimeStep) {
                    remaining -= physicsTimeStep;
                    physicsCallback(physicsTimeEngine);
                    catch_up++;
                }
                physicsTimeEngine.delta = remaining;
                physicsCallback(physicsTimeEngine);
                catch_up++;
                if (logPhysics) LOG_INFO_MESSAGE(std::fixed, "delta caught up by ", catch_up, " frame", catch_up == 1 ? "" : "s");
                physicsTimeEngine.resetTimer();
            }
        }
    });
}

void TimeEngine::stopPhysicsThread() {
    if (!running.load()) return;
    running.store(false);
    physicsThread.join();
}
