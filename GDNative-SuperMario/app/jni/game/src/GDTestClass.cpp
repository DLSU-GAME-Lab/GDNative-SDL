#include "GDTestClass.h"
#include "header.h"
#include <sstream>

GDTestClass::GDTestClass(int someValue, std::string someName)
{
	this->value = someValue;
	this->someName = someName;
}

void GDTestClass::doSomething()
{
    //only the following logs are supported in Android Logcat
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Test info for GDTest Class. Some value: %d Some Name: %s", this->value, this->someName.c_str());
    SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Test warn for GDTest Class. Some value: %d Some Name: %s", this->value, this->someName.c_str());
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Test error for GDTest Class. Some value: %d Some Name: %s", this->value, this->someName.c_str());
    SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Test critical error for GDTest Class. Some value: %d Some Name: %s", this->value, this->someName.c_str());

    std::stringstream ss;
    ss << "Test function for GDTest Class. Some value: " << this->value << " Some Name: " << this->someName;
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "GDTestClass", ss.str().c_str(), nullptr);
}
