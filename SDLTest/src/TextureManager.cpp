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
#include "ManifestLoader.h"

// load: per-file cost dominated by I/O and decoding. Algorithmic container
// insertions are O(1). As number of textures V increases, memory usage and
// any global iterations over vecTexture scale with V.
void TextureManager::load(std::string strFolderPath, std::string strName)
{
    std::string strPath = strFolderPath;
    SDL_Surface* surface = nullptr;

#ifdef __ANDROID__
    // Android: strip Assets/ prefix for asset manager
    if (strPath.rfind("Assets/", 0) == 0)
        strPath = strPath.substr(strlen("Assets/"));
#else
    // Desktop: ensure Assets/ prefix is present
    if (strPath.rfind("Assets/", 0) != 0)
        strPath = "Assets/" + strPath;
#endif

#ifdef __ANDROID__
    SDL_IOStream* io = SDL_IOFromFile(strPath.c_str(), "rb");
    if (!io) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_IOFromFile failed for '%s' : %s", strPath.c_str(), SDL_GetError());
    } else {
        surface = IMG_Load_IO(io, true); // closeio = true -> SDL will close io for us
        if (!surface) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "IMG_Load_IO failed for '%s' : %s", strPath.c_str(), SDL_GetError());
        }
    }
#else
    // Desktop fallback: load directly from filesystem
    surface = IMG_Load(strPath.c_str());
    if (!surface) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "IMG_Load failed for '%s' : %s", strPath.c_str(), SDL_GetError());
    }
#endif

    if (!surface) {
        std::cerr << "[ERROR] Problem loading image file [" << strPath << "] "
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

    // store texture
    this->mapTexture[strName].push_back(pTexture);
    this->vecTexture.push_back(pTexture);

    SDL_Log("Loaded texture '%s' as '%s'", strPath.c_str(), strName.c_str());
}

// loadFromFolder: O(F) where F = number of files processed. Total wall-time
// grows with sum of per-file I/O and decode costs.
void TextureManager::loadFromFolder(std::string strPath, std::string strName)
{
#ifdef __ANDROID__
    // read asset_manifest.txt (generated at build time) and load
    // every asset that starts with the requested folder prefix.
    std::string manifestFile = "asset_manifest.txt";
    auto entries = readAssetManifest(manifestFile);
    if (entries.empty()) {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "loadFromFolder: empty manifest '%s'", manifestFile.c_str());
        return;
    }

    // normalize query prefix: may be "sprites/" or "Assets/sprites/"
    std::string prefix = strPath;
    if (prefix.rfind("Assets/", 0) == 0) prefix = prefix.substr(strlen("Assets/"));
    if (prefix.size() && prefix.back() != '/') prefix.push_back('/');

    for (const auto& e : entries) {
        if (e.rfind(prefix, 0) == 0) {
            // entry starts with prefix -> load it
            this->load(e, strName); // e is asset-relative like "title_screen_pngs/Sprite_bg.png"
        }
    }
#else
    // Desktop: treat strPath as a real filesystem directory
    std::string directory = strPath;
    if (directory.rfind("Assets/", 0) == 0) {
        directory = directory.substr(strlen("Assets/"));
    }
    // If caller passed relative path, prefix with Assets/ like original behaviour
    std::string folder = "Assets/" + directory;
    if (!std::filesystem::exists(folder)) {
        std::cerr << "[ERROR] : path [" << folder << "] does not exist." << std::endl;
        return;
    }
    for (const auto& entry : std::filesystem::directory_iterator(folder)) {
        if (std::filesystem::is_regular_file(entry.status())) {
            std::string path = entry.path().generic_string();
            this->load(path, strName);
        }
    }
#endif
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