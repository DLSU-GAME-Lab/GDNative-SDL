#pragma once

#include <SDL3/SDL.h>
#include <unordered_map>
#include <vector>
#include <string>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

class TextureManager
{
#ifdef __ANDROID__
private:
    static void listAllAssets(AAssetManager* assetManager);
    static void listAnimationAssets(AAssetManager* assetManager);
    static std::string tryAlternativePaths(AAssetManager* assetManager, const std::string& originalPath);
    static std::string toLowerCase(const std::string& str);
    static std::string toUpperCase(const std::string& str);
    static std::string changeExtensionToUpper(const std::string& str);
    static std::string changeExtensionToLower(const std::string& str);
    static std::string changeExtensionToMatch(const std::string& str, const std::string& newExtension);
#endif

private:
    std::unordered_map<std::string, std::vector<SDL_Texture*>> mapTexture;
    std::vector<SDL_Texture*> vecTexture;
    SDL_Renderer* pRenderer = NULL;

public:
    void load(std::string strFilePath, std::string strName);
    //void loadFromFolder(std::string strFolderPath, std::string strName);
    void unload(std::string strName);
    std::vector<SDL_Texture*> getTexture(std::string strName, int nStart = -1, int nEnd = -1);
    SDL_Texture* get(const std::string& strName);

    /* * * * * * * * * * * * * * * * * * * * *
     *       SINGLETON-RELATED CONTENT       *
     * * * * * * * * * * * * * * * * * * * * */
private:
    static TextureManager* P_SHARED_INSTANCE;

private:
    TextureManager() {};
    TextureManager(const TextureManager&) {};
    TextureManager& operator=(const TextureManager&) {};

public:
    static void initialize(SDL_Renderer* pRenderer);
    static void destroy();

    static TextureManager* getInstance();
    /* * * * * * * * * * * * * * * * * * * * */
};
