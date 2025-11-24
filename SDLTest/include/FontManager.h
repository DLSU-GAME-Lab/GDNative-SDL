#pragma once
#include "SDL3/SDL_test_font.h"
#include "SDL3_ttf/SDL_ttf.h"
#include "unordered_map"
#include "iostream"
#include "string"
class FontManager
{
private:
    std::unordered_map<std::string, TTF_Font*>  mapFonts;

public:
    // use this
    TTF_Font* getFont(const std::string& fontKey, int fontSize);
    void loadFont(const std::string& fileName, const std::string& fontKey, int fontSize);
    void unloadFontFamily(const std::string& fontKey);
    void unloadAllFonts();
    
    /* * * * * * * * * * * * * * * * * * * * *
   *       SINGLETON-RELATED CONTENT       *
   * * * * * * * * * * * * * * * * * * * * */
private:
    static FontManager* P_SHARED_INSTANCE;

private:
    FontManager() {};
    FontManager(const FontManager&) {};
    FontManager& operator = (const FontManager&) {};

public:
    static void initialize();
    static void destroy();

    static FontManager* getInstance();
    /* * * * * * * * * * * * * * * * * * * * */
};
