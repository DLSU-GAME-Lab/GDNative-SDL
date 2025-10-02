#include "TextureManager.h"
#include "FontManager.h"
#include <SDL3_image/SDL_image.h>
#include "SDL3_ttf/SDL_ttf.h"
#include <iostream>
#include <algorithm>
#include <filesystem>

#ifdef __ANDROID__
#include "spdlog/spdlog.h"
#include "spdlog/sinks/android_sink.h"
#define LOG_DEBUG(...) spdlog::debug(__VA_ARGS__)
#define LOG_ERROR(...) spdlog::error(__VA_ARGS__)
#else
// on windows/linux: just no-op or printf
#include <cstdio>
#define LOG_DEBUG(...) printf(__VA_ARGS__); printf("\n")
#define LOG_ERROR(...) printf(__VA_ARGS__); printf("\n")
#endif

// load texture (handles android and desktop separately)
void TextureManager::load(std::string strFolderPath, std::string strName)
{
    SDL_Surface* surface = nullptr;

#if defined(__ANDROID__)
    // android: assets are packaged directly, no "Assets/" prefix
    std::string basePath = strFolderPath;
#else
    // desktop: keep using "Assets/" folder
    std::string basePath = "Assets/" + strFolderPath;
#endif

    LOG_DEBUG("[TextureManager] load() requested: '{}' -> name='{}'", basePath, strName);

#if defined(__ANDROID__)
    // try multiple path variants for case and extension
    std::vector<std::string> tries;
    tries.push_back(basePath);

    auto extPos = basePath.find_last_of('.');
    if (extPos != std::string::npos) {
        std::string nameNoExt = basePath.substr(0, extPos);
        std::string ext = basePath.substr(extPos);
        std::string extLower = ext, extUpper = ext;
        std::transform(extLower.begin(), extLower.end(), extLower.begin(), ::tolower);
        std::transform(extUpper.begin(), extUpper.end(), extUpper.begin(), ::toupper);
        if (extLower != ext) tries.push_back(nameNoExt + extLower);
        if (extUpper != ext) tries.push_back(nameNoExt + extUpper);
    } else {
        tries.push_back(basePath + ".png");
        tries.push_back(basePath + ".PNG");
    }

    // remove duplicate paths
    std::vector<std::string> uniqueTries;
    for (auto &p : tries) {
        if (std::find(uniqueTries.begin(), uniqueTries.end(), p) == uniqueTries.end())
            uniqueTries.push_back(p);
    }

    bool loaded = false;
    for (auto &pathTry : uniqueTries) {
        spdlog::debug("[TextureManager] trying '{}'", pathTry);

        SDL_IOStream* file = SDL_IOFromFile(pathTry.c_str(), "rb");
        if (!file) {
            spdlog::debug("[TextureManager] SDL_IOFromFile failed '{}': {}", pathTry, SDL_GetError());
            continue;
        }

        surface = IMG_Load_IO(file, true);
        if (!surface) {
            spdlog::debug("[TextureManager] IMG_Load_IO failed '{}': {}", pathTry, SDL_GetError());
            continue;
        }

        spdlog::debug("[TextureManager] loaded '{}' surface={} w={} h={}", pathTry, (void*)surface, surface->w, surface->h);
        loaded = true;
        break;
    }

    if (!loaded) {
        spdlog::error("[TextureManager] failed to load '{}'. put file in app/src/main/assets. path is case-sensitive.", basePath);
        return;
    }

#else
    // desktop: try exact and extension variants
    std::vector<std::string> tries = { basePath };
    auto extPos = basePath.find_last_of('.');
    if (extPos == std::string::npos) {
        tries.push_back(basePath + ".png");
        tries.push_back(basePath + ".PNG");
    } else {
        std::string nameNoExt = basePath.substr(0, extPos);
        std::string ext = basePath.substr(extPos);
        std::string extLower = ext, extUpper = ext;
        std::transform(extLower.begin(), extLower.end(), extLower.begin(), ::tolower);
        std::transform(extUpper.begin(), extUpper.end(), extUpper.begin(), ::toupper);
        if (extLower != ext) tries.push_back(nameNoExt + extLower);
        if (extUpper != ext) tries.push_back(nameNoExt + extUpper);
    }

    for (auto &pathTry : tries) {
        LOG_DEBUG("[TextureManager] trying '{}'", pathTry);
        surface = IMG_Load(pathTry.c_str());
        if (surface) {
            LOG_DEBUG("[TextureManager] loaded '{}' surface={} w={} h={}", pathTry, (void*)surface, surface->w, surface->h);
            break;
        } else {
            LOG_DEBUG("[TextureManager] IMG_Load failed '{}': {}", pathTry, SDL_GetError());
        }
    }

    if (!surface) {
        LOG_DEBUG("[TextureManager] failed to load '{}'", basePath);
        return;
    }
#endif

    // make texture from surface
    SDL_Texture* pTexture = SDL_CreateTextureFromSurface(this->pRenderer, surface);
    SDL_DestroySurface(surface);

    if (!pTexture) {
        LOG_ERROR("[TextureManager] SDL_CreateTextureFromSurface failed '{}': {}", basePath, SDL_GetError());
        return;
    }

    LOG_DEBUG("[TextureManager] created texture ptr={} name='{}'", (void*)pTexture, strName);
    this->mapTexture[strName].push_back(pTexture);
    this->vecTexture.push_back(pTexture);
}

void TextureManager::loadFromText(std::string strName, std::string fontType, std::string textureText, SDL_Color textColor)
{
    SDL_Surface* textSurface = TTF_RenderText_Blended(FontManager::getInstance()->getFont(fontType), textureText.c_str(), 0, textColor);
    if (textSurface == nullptr)
    {
        std::cout<<"[ERROR]: Could not render text." << SDL_GetError() << std::endl;
        return;
    }
    SDL_Texture* pTexture = SDL_CreateTextureFromSurface(this->pRenderer, textSurface);
    SDL_DestroySurface(textSurface);
    if (!pTexture) {
        std::cerr << "[ERROR] : Failed to create texture for [" << strName << "] "
            << "Error: " << SDL_GetError() << std::endl;
        return;
    }
    this->mapTexture[strName].push_back(pTexture);
    this->vecTexture.push_back(pTexture);
}
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
    std::cout << "Unloaded: " << strName << std::endl;
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
