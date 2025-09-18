#include "TextureManager.h"
#include <SDL3_image/SDL_image.h>
#include <iostream>
#include <algorithm>
#include <filesystem>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/android_sink.h"

#ifdef __ANDROID__
#include <jni.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <SDL3/SDL_system.h>   // SDL_AndroidGetJNIEnv, SDL_AndroidGetActivity
#endif

void TextureManager::load(std::string strFolderPath, std::string strName)
{
#ifdef __ANDROID__
    static AAssetManager* sAssetMgr = nullptr;
    static bool assetsListed = false;

    if (!sAssetMgr) {
        // Get Android JNI environment and activity
        JNIEnv* env = (JNIEnv*)SDL_GetAndroidJNIEnv();
        jobject activity = (jobject)SDL_GetAndroidActivity();

        if (!env || !activity) {
            spdlog::error("[Android] Failed to get JNI environment or activity");
            return;
        }

        jclass activityClass = env->GetObjectClass(activity);
        jmethodID getAssets = env->GetMethodID(activityClass, "getAssets", "()Landroid/content/res/AssetManager;");

        if (!getAssets) {
            spdlog::error("[Android] Failed to get getAssets method");
            return;
        }

        jobject assetManagerObj = env->CallObjectMethod(activity, getAssets);
        sAssetMgr = AAssetManager_fromJava(env, assetManagerObj);

        if (!sAssetMgr) {
            spdlog::error("[Android] Failed to obtain AAssetManager!");
            return;
        }

        spdlog::info("[Android] AAssetManager obtained successfully");

        // List available assets for debugging
        if (!assetsListed) {
            listAllAssets(sAssetMgr);
            listAnimationAssets(sAssetMgr);
            assetsListed = true;
        }
    }

    // Debug: Log what we're trying to load
    spdlog::debug("[Android] Attempting to load asset: {}", strFolderPath);

    // Try to open the asset
    AAsset* asset = AAssetManager_open(sAssetMgr, strFolderPath.c_str(), AASSET_MODE_STREAMING);
    if (!asset) {
        spdlog::error("[Android] Failed to open asset [{}]", strFolderPath);

        // Try alternative paths for debugging
        std::string alternativePath = tryAlternativePaths(sAssetMgr, strFolderPath);
        if (!alternativePath.empty()) {
            // Try again with the alternative path
            asset = AAssetManager_open(sAssetMgr, alternativePath.c_str(), AASSET_MODE_STREAMING);
            if (asset) {
                spdlog::info("[Android] Successfully opened alternative path: {}", alternativePath);
            } else {
                spdlog::error("[Android] Alternative path also failed: {}", alternativePath);
                return;
            }
        } else {
            return;
        }
    }

    off_t size = AAsset_getLength(asset);
    if (size <= 0) {
        spdlog::error("[Android] Asset [{}] has zero size", strFolderPath);
        AAsset_close(asset);
        return;
    }

    const void* buffer = AAsset_getBuffer(asset);
    if (!buffer) {
        spdlog::error("[Android] Failed to get buffer for [{}]", strFolderPath);
        AAsset_close(asset);
        return;
    }

    // Create SDL IO stream from memory buffer
    SDL_IOStream* io = SDL_IOFromConstMem(buffer, size);
    if (!io) {
        spdlog::error("[Android] Failed to create IO stream for [{}]: {}", strFolderPath, SDL_GetError());
        AAsset_close(asset);
        return;
    }

    // Load image from IO stream
    SDL_Surface* surface = IMG_Load_IO(io, true); // SDL_TRUE auto-closes the IO stream
    AAsset_close(asset);

    if (!surface) {
        spdlog::error("[Android] IMG_Load_IO failed for [{}]: {}", strFolderPath, SDL_GetError());
        return;
    }

    spdlog::info("[Android] Successfully loaded asset: {}", strFolderPath);

// DESKTOP/SDL ASSET LOADING SETTINGS
#else
    std::string strPath = "Assets/" + strFolderPath;
    spdlog::debug("Attempting to load texture: {}", strPath);

    SDL_Surface *surface = IMG_Load(strPath.c_str());
    if (!surface) {
        spdlog::error("Problem loading image file [{}] Error: {}", strPath, SDL_GetError());
        return;
    }
#endif
    SDL_Texture *pTexture = SDL_CreateTextureFromSurface(this->pRenderer, surface);
    SDL_DestroySurface(surface);

    if (!pTexture) {
        spdlog::error("Failed to create texture for [{}] Error: {}", strName, SDL_GetError());
        return;
    }

    this->mapTexture[strName].push_back(pTexture);
    this->vecTexture.push_back(pTexture);
    spdlog::info("Texture loaded successfully: {}", strName);
}

#ifdef __ANDROID__
void TextureManager::listAllAssets(AAssetManager* assetManager) {
    spdlog::info("[Android] Listing available assets...");

    // List root assets
    AAssetDir* assetDir = AAssetManager_openDir(assetManager, "");
    if (assetDir) {
        const char* filename;
        while ((filename = AAssetDir_getNextFileName(assetDir)) != nullptr) {
            spdlog::info("[Android]   Root: {}", filename);
        }
        AAssetDir_close(assetDir);
    }

    // List animations folder
    assetDir = AAssetManager_openDir(assetManager, "animations");
    if (assetDir) {
        const char* filename;
        spdlog::info("[Android] Contents of 'animations' folder:");
        while ((filename = AAssetDir_getNextFileName(assetDir)) != nullptr) {
            spdlog::info("[Android]   animations/{}", filename);
        }
        AAssetDir_close(assetDir);
    } else {
        spdlog::warn("[Android] 'animations' folder not found");
    }
}

void TextureManager::listAnimationAssets(AAssetManager* assetManager) {
    spdlog::info("[Android] Checking animation folders...");

    // List title_scene/player folder
    AAssetDir* playerDir = AAssetManager_openDir(assetManager, "animations/title_scene/player");
    if (playerDir) {
        spdlog::info("[Android] Contents of 'animations/title_scene/player':");
        const char* filename;
        while ((filename = AAssetDir_getNextFileName(playerDir)) != nullptr) {
            spdlog::info("[Android]   {}", filename);
        }
        AAssetDir_close(playerDir);
    } else {
        spdlog::warn("[Android] 'animations/title_scene/player' folder not found or empty");
    }

    // List title_scene/fairy folder
    AAssetDir* fairyDir = AAssetManager_openDir(assetManager, "animations/title_scene/fairy");
    if (fairyDir) {
        spdlog::info("[Android] Contents of 'animations/title_scene/fairy':");
        const char* filename;
        while ((filename = AAssetDir_getNextFileName(fairyDir)) != nullptr) {
            spdlog::info("[Android]   {}", filename);
        }
        AAssetDir_close(fairyDir);
    } else {
        spdlog::warn("[Android] 'animations/title_scene/fairy' folder not found or empty");
    }

    // List title_scene/librarian folder
    AAssetDir* librarianDir = AAssetManager_openDir(assetManager, "animations/title_scene/librarian");
    if (librarianDir) {
        spdlog::info("[Android] Contents of 'animations/title_scene/librarian':");
        const char* filename;
        while ((filename = AAssetDir_getNextFileName(librarianDir)) != nullptr) {
            spdlog::info("[Android]   {}", filename);
        }
        AAssetDir_close(librarianDir);
    } else {
        spdlog::warn("[Android] 'animations/title_scene/librarian' folder not found or empty");
    }
}

std::string TextureManager::tryAlternativePaths(AAssetManager* assetManager, const std::string& originalPath) {
    spdlog::warn("[Android] Trying alternative paths for: {}", originalPath);

    // Try different case variations
    std::vector<std::string> testPaths = {
            originalPath,
            toLowerCase(originalPath),
            toUpperCase(originalPath),
            changeExtensionToUpper(originalPath),  // .png -> .PNG
            changeExtensionToLower(originalPath),  // .PNG -> .png
            "assets/" + originalPath,
            "Assets/" + originalPath,
            "assets/" + changeExtensionToUpper(originalPath),
            "Assets/" + changeExtensionToUpper(originalPath),
            "assets/" + changeExtensionToLower(originalPath),
            "Assets/" + changeExtensionToLower(originalPath)
    };

    // Also try different directory separators and cases
    std::string pathWithForwardSlash = originalPath;
    std::replace(pathWithForwardSlash.begin(), pathWithForwardSlash.end(), '\\', '/');
    if (pathWithForwardSlash != originalPath) {
        testPaths.push_back(pathWithForwardSlash);
        testPaths.push_back(changeExtensionToUpper(pathWithForwardSlash));
        testPaths.push_back(changeExtensionToLower(pathWithForwardSlash));
    }

    for (const auto& path : testPaths) {
        AAsset* testAsset = AAssetManager_open(assetManager, path.c_str(), AASSET_MODE_STREAMING);
        if (testAsset) {
            spdlog::info("[Android] Found alternative path: {}", path);
            AAsset_close(testAsset);
            return path;
        }
    }

    // Additional debug: list what's actually in the parent directory
    size_t lastSlash = originalPath.find_last_of('/');
    if (lastSlash != std::string::npos) {
        std::string parentDir = originalPath.substr(0, lastSlash);
        spdlog::info("[Android] Listing contents of directory: {}", parentDir);
        AAssetDir* dir = AAssetManager_openDir(assetManager, parentDir.c_str());
        if (dir) {
            const char* filename;
            while ((filename = AAssetDir_getNextFileName(dir)) != nullptr) {
                spdlog::info("[Android]   Found file: {}", filename);
            }
            AAssetDir_close(dir);
        }
    }

    spdlog::error("[Android] No alternative paths worked for: {}", originalPath);
    return "";
}

std::string TextureManager::toLowerCase(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

std::string TextureManager::toUpperCase(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}

std::string TextureManager::changeExtensionToUpper(const std::string& path) {
    size_t dotPos = path.find_last_of('.');
    if (dotPos != std::string::npos) {
        std::string base = path.substr(0, dotPos);
        return base + ".PNG";
    }
    return path;
}

std::string TextureManager::changeExtensionToLower(const std::string& path) {
    size_t dotPos = path.find_last_of('.');
    if (dotPos != std::string::npos) {
        std::string base = path.substr(0, dotPos);
        return base + ".png";
    }
    return path;
}

std::string TextureManager::changeExtensionToMatch(const std::string& path, const std::string& newExtension) {
    size_t dotPos = path.find_last_of('.');
    if (dotPos != std::string::npos) {
        std::string base = path.substr(0, dotPos);
        return base + newExtension;
    }
    return path;
}
#endif

//
//void TextureManager::loadFromFolder(std::string strFolderPath, std::string strName)
//{
//    std::string strPath = "Assets/" + strFolderPath;
//    for (const auto& entry : std::filesystem::directory_iterator(strPath))
//    {
//        std::string path = entry.path().generic_string();
//        std::vector<std::string> tokens = StringUtils::split(path, '/');
//        std::string assetName = StringUtils::split(tokens[tokens.size() - 1], '.')[0];
//
//    }
//}

void TextureManager::unload(std::string strName)
{
    // destroy each texture associated with this name
    for (SDL_Texture* tex : this->mapTexture[strName]) {
        SDL_DestroyTexture(tex);
    }

    // remove them from the global vector
    this->vecTexture.erase(
        std::remove_if(this->vecTexture.begin(), this->vecTexture.end(),
            [&](SDL_Texture* tex) {
                return std::find(this->mapTexture[strName].begin(),
                    this->mapTexture[strName].end(),
                    tex) != this->mapTexture[strName].end();
            }),
        this->vecTexture.end()
    );

    this->mapTexture[strName].clear();
}

std::vector<SDL_Texture*> TextureManager::getTexture(std::string strName, int nStart, int nEnd)
{
    if (mapTexture.find(strName) == mapTexture.end())
        return {};

    if (nStart == -1)
        return mapTexture[strName];

    std::vector<SDL_Texture*> vecTexture = mapTexture[strName];
    if (nEnd == -1)
        nEnd = nStart + 1;

    if (nStart >= 0 && nEnd <= (int)vecTexture.size())
        return std::vector<SDL_Texture*>(vecTexture.begin() + nStart, vecTexture.begin() + nEnd);

    return {};
}

// get a single texture by name (first one in the list)
SDL_Texture* TextureManager::get(const std::string& strName)
{
    if (mapTexture.find(strName) != mapTexture.end() && !mapTexture[strName].empty()) {
        return mapTexture[strName][0];
    }
    return nullptr;
}

/* * * * * * * * * * * * * * * * * * * * * 
 *       SINGLETON-RELATED CONTENT       * 
 * * * * * * * * * * * * * * * * * * * * */
TextureManager* TextureManager::P_SHARED_INSTANCE = NULL;

void TextureManager::initialize(SDL_Renderer* pRenderer)
{
    P_SHARED_INSTANCE = new TextureManager();
    P_SHARED_INSTANCE->pRenderer = pRenderer;
}

void TextureManager::destroy()
{
    delete P_SHARED_INSTANCE;
}

TextureManager* TextureManager::getInstance()
{
    return P_SHARED_INSTANCE;
}
/* * * * * * * * * * * * * * * * * * * * */
