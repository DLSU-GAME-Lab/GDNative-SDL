#include "FontManager.h"
#include <fstream>
#include <cstdio>
#include "ManifestLoader.h"

TTF_Font* FontManager::getFont(const std::string fontKey, int fontSize)
{
    std::string fullKey = fontKey + "_" + std::to_string(fontSize);
    auto it = mapFonts.find(fullKey);
    if (it != mapFonts.end()) return it->second;

    SDL_LogError(
            SDL_LOG_CATEGORY_APPLICATION,
            "[ERROR] Font [%s] not found.",
            fullKey.c_str()
            );
    return nullptr;
}

void FontManager::loadFont(const std::string fileName, const std::string fontKey, int fontSize)
{
#if defined(__ANDROID__)
    const std::string assetPath = "Fonts/" + fileName;
#else
    const std::string assetPath = "Assets/Fonts/" + fileName;
#endif
    std::string fullKey = fontKey + "_" + std::to_string(fontSize);

    if (mapFonts.contains(fullKey)) return;

    // Try to load raw bytes (works for APK assets)
    size_t size = 0;
    void* data = SDL_LoadFile(assetPath.c_str(), &size);
    if (!data || size <= 0) {
        SDL_LogError(
                SDL_LOG_CATEGORY_APPLICATION,
                "[ERROR] TTF: SDL_LoadFile failed for %s : %s ",
                assetPath.c_str(),
                SDL_GetError()
                );
        if (data) SDL_free(data);
        return;
    }

    // Determine writable temporary directory
    char* pref = SDL_GetPrefPath("org.main", "babaylan_tales");
    std::string tmpDir = pref ? std::string(pref) : std::string("/data/local/tmp/");
    if (pref) SDL_free(pref);

    // Make a safe filename from assetPath
    std::string safeName = assetPath;
    for (char &c : safeName) if (c == '/' || c == '\\') c = '_';
    std::string tmpPath = tmpDir + safeName;

    // Write bytes to temp file
    {
        std::ofstream ofs(tmpPath, std::ios::binary);
        if (!ofs) {
            std::cerr << "[ERROR] TTF: failed to open temp file " << tmpPath << " for writing\n";
            SDL_free(data);
            return;
        }
        ofs.write(static_cast<char*>(data), static_cast<std::streamsize>(size));
    }

    SDL_free(data);

    // Load font from temp file using TTF_OpenFont
    TTF_Font* font = TTF_OpenFont(tmpPath.c_str(), fontSize);
    if (!font) {
        SDL_LogError(
                SDL_LOG_CATEGORY_APPLICATION,
                "[ERROR] Failed to load font [%s] : %s",
                fullKey.c_str(),
                SDL_GetError()
        );
        // best-effort cleanup
        std::remove(tmpPath.c_str());
        return;
    }

    mapFonts[fullKey] = font;

    // remove temp file (best-effort)
    std::remove(tmpPath.c_str());

    SDL_Log("[DEBUG] %s has been loaded", fullKey.c_str());
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
}

void FontManager::destroy()
{
    delete P_SHARED_INSTANCE;
}

FontManager* FontManager::getInstance()
{
    return P_SHARED_INSTANCE;
}
/* * * * * * * * * * * * * * * * * * * * */
