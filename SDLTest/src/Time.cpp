#include "Time.h"

Time* Time::P_SHARED_INSTANCE = NULL;

void Time::initialize()
{
    P_SHARED_INSTANCE = new Time();
}

void Time::destroy()
{
    delete P_SHARED_INSTANCE;
}

void Time::logFrameStart()
{
    P_SHARED_INSTANCE->start = std::chrono::system_clock::now();
}

void Time::logFrameEnd()
{
    P_SHARED_INSTANCE->end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsedSeconds = P_SHARED_INSTANCE->end - P_SHARED_INSTANCE->start;

    P_SHARED_INSTANCE->unscaledDeltaTime = elapsedSeconds.count();
    P_SHARED_INSTANCE->deltaTime = elapsedSeconds.count() * P_SHARED_INSTANCE->timeScale;

    P_SHARED_INSTANCE->time += P_SHARED_INSTANCE->deltaTime;
    P_SHARED_INSTANCE->unscaledTime += P_SHARED_INSTANCE->unscaledDeltaTime;
}

double Time::getTime()
{
    return P_SHARED_INSTANCE->time;
}

double Time::getDeltaTime()
{
    return P_SHARED_INSTANCE->deltaTime;
}

double Time::getUnscaledTime()
{
    return P_SHARED_INSTANCE->unscaledTime;
}

double Time::getUnscaledDeltaTime()
{
    return P_SHARED_INSTANCE->unscaledDeltaTime;
}

void Time::setTimeScale(double timeScale)
{
    P_SHARED_INSTANCE->timeScale = timeScale;
}
