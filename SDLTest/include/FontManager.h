#pragma once
#include "SDL3/SDL.h"
#include "SDL3/SDL_test_font.h"
#include "SDL3_ttf/SDL_ttf.h"
#include "unordered_map"
#include "iostream"
#include "string"
class FontManager
{
private:
    struct FontFile
    {
        void* pData;
        size_t nSize;
    };

    std::unordered_map<std::string, TTF_Font*>  mapFonts;
    // raw font files, kept alive while any font opened from them exists
    std::unordered_map<std::string, FontFile> mapFontFiles;

public:
    // use this
    TTF_Font* getFont(const std::string& fontKey, int fontSize);
    void loadFont(const std::string& fileName, const std::string& fontKey, int fontSize);
    void unloadFontFamily(const std::string& fontKey);
    void unloadAllFonts();

    // text helpers
    static std::string getFontKey(const std::string& fileName);
    SDL_Texture* createTextTexture(SDL_Renderer* pRenderer, const std::string& fontKey, int fontSize, const std::string& strText, SDL_Color color, int wrapWidth = 0);

    /* * * * * * * * * * * * * * * * * * * * *
   *       SINGLETON-RELATED CONTENT       *
   * * * * * * * * * * * * * * * * * * * * */
private:
    static FontManager* P_SHARED_INSTANCE;

private:
    FontManager() {};
    FontManager(const FontManager&) = delete;
    FontManager& operator = (const FontManager&) = delete;

public:
    static void initialize();
    static void destroy();

    static FontManager* getInstance();
    /* * * * * * * * * * * * * * * * * * * * */
};