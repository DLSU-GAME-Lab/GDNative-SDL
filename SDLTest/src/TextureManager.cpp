#include "TextureManager.h"

void TextureManager::load(std::string strName)
{
    std::string strPath = "Resource/Asset/" + strName;
    SDL_Texture* pTexture = new SDL_Texture();

    //if(pTexture->loadFromFile(strPath)) {
    //    this->mapTexture[strName].push_back(pTexture);
    //    this->vecTexture.push_back(pTexture);
    //}
    //else std::cout << "[ERROR] : Problem loading image file [" + strName + "] from Asset folder." << std::endl;
    //
}

void TextureManager::unload(std::string strName)
{
    int nFinalSize = this->vecTexture.size() - this->mapTexture[strName].size();
    int i = 0;
    while(this->vecTexture.size() > nFinalSize) {
        bool bErased = false;
        for(int j = 0; j < this->mapTexture[strName].size() && !bErased; j++) {
            if(this->vecTexture[i] == this->mapTexture[strName][j]) {
                this->vecTexture.erase(this->vecTexture.begin() + i);
                bErased = true;
            }
        }
        if(!bErased) i++;
    }
    this->mapTexture[strName].clear();
}

std::vector<SDL_Texture*> TextureManager::getTexture(std::string strName, int nStart, int nEnd)
{
    /*
    if(nStart == -1)
        return this->mapTexture[strName];

    else {
        std::vector<SDL_Texture*> vecTexture = this->mapTexture[strName];
        std::vector<SDL_Texture*>::const_iterator iStart = vecTexture.begin() + nStart;

        if(nEnd == -1)
            nEnd = nStart + 1;

        std::vector<SDL_Texture*>::const_iterator iEnd = vecTexture.begin() + nEnd + 1;
        std::vector<SDL_Texture*> vecSublist = std::vector(iStart, iEnd);

        return vecSublist;
    }*/

}

/* * * * * * * * * * * * * * * * * * * * * 
 *       SINGLETON-RELATED CONTENT       * 
 * * * * * * * * * * * * * * * * * * * * */
TextureManager* TextureManager::P_SHARED_INSTANCE = NULL;

void TextureManager::initialize()
{
    P_SHARED_INSTANCE = new TextureManager();
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
