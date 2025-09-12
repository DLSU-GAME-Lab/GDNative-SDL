#pragma once
#include <iostream>
#include <SDL3/SDL.h>

class BaseRunner;

class EngineTime
{
public:
	Uint64 getTrueDeltaTime();
	double getTime();
	double getDeltaTime();
	double getUnscaledTime();
	double getUnscaledDeltaTime();
	void setTimeScale(double dTimeScale);

private:
	Uint64 tStart = 0;
	Uint64 tEnd = 0;
	Uint64 tDeltaTime = 0;

	double dTime = 0.0;
	double dDeltaTime = 0.0;
	double dUnscaledTime = 0.0;
	double dUnscaledDeltaTime = 0.0;
	double dTimeScale = 1.0;

	static void logFrameStart();
	static void logFrameEnd();

	/* * * * * * * * * * * * * * * * * * * * *
	 *       SINGLETON-RELATED CONTENT       *
	 * * * * * * * * * * * * * * * * * * * * */
private:
	static EngineTime* P_SHARED_INSTANCE;

private:
	EngineTime() {};
	EngineTime(const EngineTime&) {};
	EngineTime& operator=(const EngineTime&) {};

public:
	static void initialize();
	static void destroy();

	static EngineTime* getInstance();
	/* * * * * * * * * * * * * * * * * * * * */

	friend class Runner;
};

