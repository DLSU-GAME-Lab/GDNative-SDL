#include "header.h"
#include <memory>
//#include "GDCore.h"
#include "GDCore.h"

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO) != 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_Init failed (%s)", SDL_GetError());
    }

    if (SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Hello World",
                                  "!! Your SDL project successfully runs on Android !!", NULL) != 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_ShowSimpleMessageBox failed (%s)", SDL_GetError());
    }
//
//    std::unique_ptr<GDTestClass> gdTestClass;
//    gdTestClass = std::make_unique<GDTestClass>(99, "DLSU-GAMELAB");
//    gdTestClass->doSomething();
//
//    SDL_Quit();
//    return 0;

//	CCore oCore;
//	oCore.mainLoop();
//
//	return 0;

    GDCore gdCore;
    gdCore.mainLoop();

    return 0;
}
