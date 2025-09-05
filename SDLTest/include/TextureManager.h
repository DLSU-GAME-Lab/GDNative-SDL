#pragma once

#include <SDL3/SDL.h>
#include <unordered_map>
#include <vector>
#include <string>

class TextureManager
{
private:
    std::unordered_map<std::string, std::vector<SDL_Texture*>> mapTexture;
    std::vector<SDL_Texture*> vecTexture;

public:
    void load(std::string strName, SDL_Renderer* renderer);
    void unload(std::string strName);
    std::vector<SDL_Texture*> getTexture(std::string strName, int nStart = -1, int nEnd = -1);
    SDL_Texture* get(const std::string& strName);

    /* * * * * * * * * * * * * * * * * * * * *
     *       SINGLETON-RELATED CONTENT       *
     * * * * * * * * * * * * * * * * * * * * */
private:
    static TextureManager* P_SHARED_INSTANCE;

private:
    TextureManager() {};
    TextureManager(const TextureManager&) {};
    TextureManager& operator=(const TextureManager&) {};

public:
    static void initialize();
    static void destroy();

    static TextureManager* getInstance();
    /* * * * * * * * * * * * * * * * * * * * */
};
