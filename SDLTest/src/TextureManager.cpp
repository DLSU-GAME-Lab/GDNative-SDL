#include "TextureManager.h"
#include <SDL3_image/SDL_image.h>
#include <iostream>
#include <algorithm>
#include <filesystem>

void TextureManager::load(std::string strFolderPath, std::string strName)
{
    std::string strPath = "Assets/" + strFolderPath;
    // DEBUG: print what path is being loaded
    std::cout << "[DEBUG] Attempting to load texture: " << strPath << std::endl;

    SDL_Surface* surface = IMG_Load(strPath.c_str());
    if (!surface) {
        std::cerr << "[ERROR] : Problem loading image file [" << strPath << "] "
            << "Error: " << SDL_GetError() << std::endl;
        return;
    }

    SDL_Texture* pTexture = SDL_CreateTextureFromSurface(this->pRenderer, surface);
    SDL_DestroySurface(surface);

    if (!pTexture) {
        std::cerr << "[ERROR] : Failed to create texture for [" << strPath << "] "
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
