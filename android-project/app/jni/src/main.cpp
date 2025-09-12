#include "SDL3/SDL.h"
#include "Runner.h"
#include <memory>
#include "SDL3_image/SDL_image.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/android_sink.h"
#define MAIN_TAG "main"
void run_main_loop(SDL_Window* window);
extern "C" void SDL_main(int argc, char* argv[])
{
    auto android_logger = spdlog::android_logger_mt("android", "spdlog-android");
    android_logger->set_level(spdlog::level::info);
    spdlog::set_default_logger(android_logger);
    android_logger->set_pattern("%v");

    spdlog::info("Main.cpp Running", MAIN_TAG);
    if (SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO) == 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_Init failed (%s)", SDL_GetError());
    }


    Runner runner;
    runner.run();
}


