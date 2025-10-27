// ---------------------------------------------------------------------------
// Responsibilities: load textures from disk, create cached textures, unload.
// ---------------------------------------------------------------------------

#include "TextureManager.h"
#include "FontManager.h"
#include <SDL3_image/SDL_image.h>
#include "SDL3_ttf/SDL_ttf.h"
#include "StringUtils.h"
#include <iostream>
#include <algorithm>
#include <filesystem>
#include "RendererContext.h"

// load: per-file cost dominated by I/O and decoding. Algorithmic container
// insertions are O(1). As number of textures V increases, memory usage and
// any global iterations over vecTexture scale with V.
void TextureManager::load(std::string strFolderPath, std::string strName)
{
    // O(1) setup + O(fileSize) I/O to load from disk.
    // Texture creation cost is high but fixed per file.
    std::string strPath;
    auto token = StringUtils::split(strFolderPath, '/');
    if (token[0] == "Assets") strPath = strFolderPath;
    else strPath = "Assets/" + strFolderPath;

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

    // O(1): push_back into containers.
    this->mapTexture[strName].push_back(pTexture);
    this->vecTexture.push_back(pTexture);
}

// loadFromFolder: O(F) where F = number of files processed. Total wall-time
// grows with sum of per-file I/O and decode costs.
void TextureManager::loadFromFolder(std::string strPath, std::string strName)
{
    // O(F): loops through files in a folder, calling load() for each.
    // F = number of files in folder.
    
    std::string directory = "Assets/" + strPath;
    if (!std::filesystem::exists(directory.c_str()))
    {
        std::cerr << "[ERROR] : path [" << directory << "] " << "does no exist." << std::endl;
        return;
    }
    
    for (const auto& entry : std::filesystem::directory_iterator(directory))
    {
        if (std::filesystem::is_regular_file(entry.status()))
        {
            std::string path = entry.path().generic_string();
            this->load(path, strName);
        }
    }
}

// loadFromText: O(1) algorithmically; TTF rendering cost is an expensive
// constant that affects wall-time.
void TextureManager::loadFromText(std::string strName, std::string fontType, int nFontSize, std::string textureText, SDL_Color textColor)
{
    // O(1): creates texture from text; I/O and render cost from SDL_ttf.
    SDL_Surface* textSurface = TTF_RenderText_Blended_Wrapped(FontManager::getInstance()->getFont(fontType, nFontSize), textureText.c_str(), 0, textColor, 0);
    if (textSurface == nullptr)
    {
        std::cout << "[ERROR]: Could not render text." << SDL_GetError() << std::endl;
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

// unload: current implementation destroys textures for a name and removes
// them from a global vector. Worst-case complexity can be proportional to
// the size of the global texture vector V times the number of textures T
// being removed (O(V * T)) depending on implementation details of removal.
// As V grows large, unload operations may become increasingly expensive.
void TextureManager::unload(std::string strName)
{
    // O(T): destroys all textures associated with name.
    // Erase + vector filtering also O(T) with small constants.
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

// getTexture: returning copies of vectors is O(T) (copy cost). As T increases
// this creates overhead for callers that only needed read access.
std::vector<SDL_Texture*> TextureManager::getTexture(std::string strName, int nStart, int nEnd)
{
    // O(T): copies subset of textures if range given.
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

// get: O(1) fetch of first texture pointer by name assuming map lookup is O(1).
// get a single texture by name (first one in the list)
SDL_Texture* TextureManager::get(const std::string& strName)
{
    // O(1): fetches first texture by key.
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
