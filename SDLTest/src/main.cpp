#include "SDL3/SDL.h"
#include "Runner.h"
#include <memory>

// actual program entry logic
int game_main(int argc, char* argv[])
{
    Runner runner;
    runner.run();
    return 0;
}

#if defined(__ANDROID__)
// on Android, SDLActivity looks for SDL_main
extern "C" __attribute__((visibility("default"))) int SDL_main(int argc, char* argv[])
{
    return game_main(argc, argv);
}
#else
// on desktop, SDL redefines main into SDL_main
int main(int argc, char* argv[])
{
    return game_main(argc, argv);
}
#endif
