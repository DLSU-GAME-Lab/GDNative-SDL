#include "FontManager.h"

TTF_Font* FontManager::getFont(const std::string& fontKey, int fontSize)
{
    std::string fullKey = fontKey + "_" + std::to_string(fontSize);
    auto it = mapFonts.find(fullKey);
    if (it != mapFonts.end()) return it->second;

    std::cerr << "[ERROR] Font [" << fullKey << "] not found.\n";
    return nullptr;
}

void FontManager::loadFont(const std::string& fileName, const std::string& fontKey, int fontSize)
{
    // Use the correct path that matches asset_manifest.txt
    std::string path = "Fonts/" + fileName;  // Changed from "Assets/Fonts/"
    std::string fullKey = fontKey + "_" + std::to_string(fontSize);

    if (mapFonts.contains(fullKey)) return;

    TTF_Font* font = nullptr;

#ifdef __ANDROID__
    // On Android, load font via SDL_RWops from assets
    SDL_IOStream* rw = SDL_IOFromFile(path.c_str(), "rb");
    if (!rw) {
        std::cerr << "[ERROR] Failed to open font file [" << path << "]: " << SDL_GetError() << std::endl;
        return;
    }

    // SDL_TRUE means SDL will automatically close the RWops when done
    font = TTF_OpenFontIO(rw, true, fontSize);
#else
    // On desktop, use regular file loading with Assets/ prefix
    std::string desktopPath = "Assets/" + path;
    font = TTF_OpenFont(desktopPath.c_str(), fontSize);
#endif

    if (!font) {
        std::cerr << "[ERROR] Failed to load font [" << fullKey << "]: " << SDL_GetError() << std::endl;
        return;
    }
    else
    {
        std::cout << "[DEBUG] " << fullKey << " has been loaded from: " << path << std::endl;
    }

    mapFonts[fullKey] = font;
}

void FontManager::unloadFontFamily(const std::string& fontKey)
{
    for (auto it = mapFonts.begin(); it != mapFonts.end(); ) {
        if (it->first.starts_with(fontKey + "_")) {
            TTF_CloseFont(it->second);
            it = mapFonts.erase(it);
        }
        else {
            ++it;
        }
    }
}

void FontManager::unloadAllFonts()
{
    if (mapFonts.empty())
    {
        std::cout << "[FontManager] No fonts to unload.\n";
        return;
    }
    for (auto& pair : mapFonts)
    {
        if (pair.second)
        {
            std::cout << "[DEBUG] " << pair.first << " has been removed" << std::endl;
            TTF_CloseFont(pair.second);
        }
    }

    mapFonts.clear();
    std::cout << "[FontManager] All fonts unloaded.\n";
}

/* * * * * * * * * * * * * * * * * * * * *
 *       SINGLETON-RELATED CONTENT       *
 * * * * * * * * * * * * * * * * * * * * */
FontManager* FontManager::P_SHARED_INSTANCE = NULL;

void FontManager::initialize()
{
    P_SHARED_INSTANCE = new FontManager();
    //Initialize font loading
    if (TTF_Init() == false)
    {
        SDL_Log("SDL_ttf could not initialize! SDL_ttf error: %s\n", SDL_GetError());
    }
    else
    {
        SDL_Log("SDL_ttf initialized successfully");
    }
}

void FontManager::destroy()
{
    if (P_SHARED_INSTANCE)
    {
        P_SHARED_INSTANCE->unloadAllFonts();
        delete P_SHARED_INSTANCE;
        P_SHARED_INSTANCE = nullptr;
    }
    TTF_Quit();
}

FontManager* FontManager::getInstance()
{
    return P_SHARED_INSTANCE;
}
/* * * * * * * * * * * * * * * * * * * * */