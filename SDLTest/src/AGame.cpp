#include "AGame.h"
#include "GameObjectManager.h"
#include "MetricsManager.h"
#include <iostream>
#include <string>

void AGame::processEvent(SDL_Event* pEvent)
{
	GameObjectManager::getInstance()->processInput(pEvent);
}

void AGame::initializeMetrics()
{
	std::cout << "[Runner] Initializing MetricsManager..." << std::endl;
	MetricsManager::initialize();
#if defined(__ANDROID__)
	const char* storagePath = SDL_GetAndroidExternalStoragePath();
	if (!storagePath)
	{
		storagePath = SDL_GetAndroidInternalStoragePath(); // fall back if external is unavailable
	}
	if (storagePath)
	{
		MetricsManager::getInstance()->setCSVPath(std::string(storagePath) + "/metrics_export.csv");
	}
	else
	{
		// SDL_GetError() will tell you why both failed - log it via your own logging
	}
#endif
	std::cout << "[Runner] MetricsManager initialized." << std::endl;
}
