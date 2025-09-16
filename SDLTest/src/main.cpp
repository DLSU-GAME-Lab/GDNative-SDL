#include "SDL3/SDL.h"
#include "Runner.h"
#include <memory>


int main(int argc, char* argv[])
{
    if (SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO) == 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_Init failed (%s)", SDL_GetError());
    }

  /*  if (SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Hello World",
        "!! Your SDL project successfully runs on Android !!", NULL) == 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_ShowSimpleMessageBox failed (%s)", SDL_GetError());
    }*/

    //program
    Runner runner;
    runner.run();
}