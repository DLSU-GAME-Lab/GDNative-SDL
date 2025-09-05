#pragma once

#include <iostream>
#include <chrono>
#include <ctime>

class BaseRunner;

class Time
{
public:
	static void initialize();
	static void destroy();

	static double getTime();
	static double getDeltaTime();
	static double getUnscaledTime();
	static double getUnscaledDeltaTime();
	static void setTimeScale(double timeScale);

private:
	Time() {};
	Time(Time const&) {};
	Time& operator=(Time const&) {};
	static Time* P_SHARED_INSTANCE;

	std::chrono::system_clock::time_point start;
	std::chrono::system_clock::time_point end;

	double time = 0.0;
	double deltaTime = 0.0;
	double unscaledTime = 0.0;
	double unscaledDeltaTime = 0.0;
	double timeScale = 1.0;

	static void logFrameStart();
	static void logFrameEnd();

	friend class BaseRunner;
};

