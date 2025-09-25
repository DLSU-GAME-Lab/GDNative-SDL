#include "FontManager.h"

TTF_Font* FontManager::getFont(std::string strFontName)
{
    if (this->mapFonts[strFontName] != NULL)
        return this->mapFonts[strFontName];

    else {
        std::cout << "[ERROR] : Font [" << strFontName << "] NOT found." << std::endl;
        return NULL;
    }
    
}

void FontManager::loadFont(std::string strItem, std::string strName, int dFontSize)
{
    std::string strPath = "Assets/Fonts/" + strItem;
    // DEBUG: print what path is being loaded
    std::cout << "[DEBUG] Attempting to load font: " << strPath << std::endl;
    
    TTF_Font* fontLoad;
    if (fontLoad = TTF_OpenFont(strPath.c_str(), dFontSize) ; fontLoad == nullptr)
    {
        std::cout << "[ERROR] : Font[" << strName << "] could not be loaded. " << SDL_GetError() << std::endl;
    }

    this->mapFonts[strName] = (fontLoad);
}
void FontManager::unloadFont(std::string strName)
{
    TTF_CloseFont(this->mapFonts[strName]);
    this->mapFonts.erase(strName);
    std::cout << "Font: " << strName << " Unloaded" << std::endl;
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
