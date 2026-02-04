#include "ManifestLoader.h"
#include <SDL3/SDL.h>
#include <sstream>
#include <string>
#include <vector>

#ifdef __ANDROID__
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <jni.h>

extern "C" {
JNIEXPORT void JNICALL
Java_org_libsdl_app_SDLActivity_nativeSetAssetManager(JNIEnv* env, jobject thiz, jobject asset_manager);
}

static AAssetManager* g_asset_manager = nullptr;

extern "C" JNIEXPORT void JNICALL
Java_org_libsdl_app_SDLActivity_nativeSetAssetManager(JNIEnv* env, jobject thiz, jobject asset_manager) {
    g_asset_manager = AAssetManager_fromJava(env, asset_manager);
}
#endif

std::vector<std::string> readAssetManifest(const std::string& manifestFilename)
{
    std::vector<std::string> out;

#ifdef __ANDROID__
    if (g_asset_manager) {
        AAsset* asset = AAssetManager_open(g_asset_manager, manifestFilename.c_str(), AASSET_MODE_BUFFER);
        if (asset) {
            off_t size = AAsset_getLength(asset);
            char* buffer = new char[size + 1];
            AAsset_read(asset, buffer, size);
            buffer[size] = '\0';
            AAsset_close(asset);

            std::istringstream iss(buffer);
            std::string line;
            while (std::getline(iss, line)) {
                if (!line.empty()) {
                    out.push_back(line);
                }
            }
            delete[] buffer;
        } else {
            SDL_Log("readAssetManifest: Can't open Android asset '%s'", manifestFilename.c_str());
        }
    }
#else
    // Desktop/fallback implementation
    size_t size = 0;
    void* data = SDL_LoadFile(manifestFilename.c_str(), &size);
    if (!data) {
        SDL_Log("readAssetManifest: can't open manifest '%s' : %s", manifestFilename.c_str(), SDL_GetError());
        return out;
    }

    std::string buf(static_cast<char*>(data), (size_t)size);
    SDL_free(data);

    std::istringstream iss(buf);
    std::string line;
    while (std::getline(iss, line)) {
        if (!line.empty()) {
            out.push_back(line);
        }
    }
#endif

    return out;
}