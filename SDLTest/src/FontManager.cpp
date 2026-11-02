#include "FontManager.h"

TTF_Font* FontManager::getFont(const std::string& fontKey, int fontSize)
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

void FontManager::loadFont(const std::string& fileName, const std::string& fontKey, int fontSize)
{
#if defined(__ANDROID__)
    const std::string assetPath = "Fonts/" + fileName;
#else
    const std::string assetPath = "Assets/Fonts/" + fileName;
#endif
    std::string fullKey = fontKey + "_" + std::to_string(fontSize);

    if (mapFonts.contains(fullKey)) return;

    // load the raw bytes once per file (works for APK assets), every size shares them
    auto itFile = mapFontFiles.find(fileName);
    if (itFile == mapFontFiles.end())
    {
        size_t nSize = 0;
        void* pData = SDL_LoadFile(assetPath.c_str(), &nSize);
        if (!pData || nSize == 0)
        {
            SDL_LogError(
                SDL_LOG_CATEGORY_APPLICATION,
                "[ERROR] TTF: SDL_LoadFile failed for %s : %s ",
                assetPath.c_str(),
                SDL_GetError()
            );
            if (pData) SDL_free(pData);
            return;
        }
        itFile = mapFontFiles.emplace(fileName, FontFile{ pData, nSize }).first;
    }

    // open the font straight from memory, the buffer stays alive until the fonts are unloaded
    SDL_IOStream* pIO = SDL_IOFromConstMem(itFile->second.pData, itFile->second.nSize);
    if (!pIO)
    {
        SDL_LogError(
            SDL_LOG_CATEGORY_APPLICATION,
            "[ERROR] TTF: SDL_IOFromConstMem failed for %s : %s ",
            assetPath.c_str(),
            SDL_GetError()
        );
        return;
    }

    TTF_Font* font = TTF_OpenFontIO(pIO, true, fontSize);
    if (!font)
    {
        SDL_LogError(
            SDL_LOG_CATEGORY_APPLICATION,
            "[ERROR] Failed to load font [%s] : %s",
            fullKey.c_str(),
            SDL_GetError()
        );
        return;
    }

    mapFonts[fullKey] = font;

    SDL_Log("[DEBUG] %s has been loaded from: %s", fullKey.c_str(), assetPath.c_str());
}

void FontManager::unloadFontFamily(const std::string& fontKey)
{
    for (auto it = mapFonts.begin(); it != mapFonts.end(); )
    {
        if (it->first.starts_with(fontKey + "_"))
        {
            TTF_CloseFont(it->second);
            it = mapFonts.erase(it);
        }
        else
        {
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

    // fonts are closed, the raw files can go too
    for (auto& pair : mapFontFiles)
    {
        SDL_free(pair.second.pData);
    }
    mapFontFiles.clear();

    std::cout << "[FontManager] All fonts unloaded.\n";
}

// strips the .ttf / .otf extension so a font file name can be used as a font key
std::string FontManager::getFontKey(const std::string& fileName)
{
    std::string key = fileName;

    size_t ttfPos = key.rfind(".ttf");
    size_t otfPos = key.rfind(".otf");

    if (ttfPos != std::string::npos && ttfPos == key.length() - 4)
    {
        key = key.substr(0, ttfPos);
    }
    else if (otfPos != std::string::npos && otfPos == key.length() - 4)
    {
        key = key.substr(0, otfPos);
    }

    return key;
}

// renders text into a new texture owned by the caller, returns NULL on failure.
// wrapWidth is in pixels, 0 = no wrapping (only newlines)
SDL_Texture* FontManager::createTextTexture(SDL_Renderer* pRenderer, const std::string& fontKey, int fontSize, const std::string& strText, SDL_Color color, int wrapWidth)
{
    TTF_Font* font = this->getFont(fontKey, fontSize);
    if (!font) return nullptr;

    if (strText.empty())
    {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "[FontManager] empty text for %s", fontKey.c_str());
        return nullptr;
    }

    // note: signature requires length BEFORE color
    SDL_Surface* textSurface = TTF_RenderText_Blended_Wrapped(font, strText.c_str(), strText.size(), color, wrapWidth);
    if (!textSurface)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[FontManager] TTF_Render failed: %s", SDL_GetError());
        return nullptr;
    }

    SDL_Texture* pTexture = SDL_CreateTextureFromSurface(pRenderer, textSurface);
    SDL_DestroySurface(textSurface);

    if (!pTexture)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[FontManager] CreateTextureFromSurface failed: %s", SDL_GetError());
        return nullptr;
    }

    return pTexture;
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