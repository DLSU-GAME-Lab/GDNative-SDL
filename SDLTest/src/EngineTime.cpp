#include "EngineTime.h"

void EngineTime::logFrame()
{
    P_SHARED_INSTANCE->tEnd = P_SHARED_INSTANCE->tStart;
    P_SHARED_INSTANCE->tStart = SDL_GetTicks();

    P_SHARED_INSTANCE->tDeltaTime = P_SHARED_INSTANCE->tEnd - P_SHARED_INSTANCE->tStart;
    P_SHARED_INSTANCE->dUnscaledDeltaTime = P_SHARED_INSTANCE->tDeltaTime / 1000.0;
    P_SHARED_INSTANCE->dDeltaTime = P_SHARED_INSTANCE->dUnscaledDeltaTime * P_SHARED_INSTANCE->dTimeScale;

    P_SHARED_INSTANCE->dTime += P_SHARED_INSTANCE->dDeltaTime;
    P_SHARED_INSTANCE->dUnscaledTime += P_SHARED_INSTANCE->dUnscaledDeltaTime;
}

Uint64 EngineTime::getTrueDeltaTime()
{
    return P_SHARED_INSTANCE->tDeltaTime;
}

double EngineTime::getTime()
{
    return P_SHARED_INSTANCE->dTime;
}

double EngineTime::getDeltaTime()
{
    return P_SHARED_INSTANCE->dDeltaTime;
}

double EngineTime::getUnscaledTime()
{
    return P_SHARED_INSTANCE->dUnscaledTime;
}

double EngineTime::getUnscaledDeltaTime()
{
    return P_SHARED_INSTANCE->dUnscaledDeltaTime;
}

void EngineTime::setTimeScale(double timeScale)
{
    P_SHARED_INSTANCE->dTimeScale = timeScale;
}

/* * * * * * * * * * * * * * * * * * * * *
 *       SINGLETON-RELATED CONTENT       *
 * * * * * * * * * * * * * * * * * * * * */
EngineTime* EngineTime::P_SHARED_INSTANCE = NULL;

void EngineTime::initialize()
{
    P_SHARED_INSTANCE = new EngineTime();
}

void EngineTime::destroy()
{
    delete P_SHARED_INSTANCE;
}

EngineTime* EngineTime::getInstance()
{
    return P_SHARED_INSTANCE;
}
/* * * * * * * * * * * * * * * * * * * * */
