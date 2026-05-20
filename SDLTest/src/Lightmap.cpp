#include "Lightmap.h"
#include "GameObjectManager.h"
#include "Settings.h"
#include "EmptyObject.h"
#include "RendererContext.h"

void Lightmap::drawWidget()
{
    for (Light2D* light : lights) light->drawLightWidget();
}

void Lightmap::perform()
{
    drawLightmap();

    SDL_BlendMode customBlend = SDL_ComposeCustomBlendMode(
            SDL_BLENDFACTOR_ZERO,                // srcColorFactor
            SDL_BLENDFACTOR_SRC_COLOR,           // dstColorFactor <- multiplies scene by lightmap
            SDL_BLENDOPERATION_ADD,              // colorOperation
            SDL_BLENDFACTOR_ONE,                 // srcAlphaFactor
            SDL_BLENDFACTOR_ONE_MINUS_SRC_ALPHA, // dstAlphaFactor
            SDL_BLENDOPERATION_ADD               // alphaOperation
    );

    SDL_SetTextureBlendMode(this->pLightmapTex, customBlend);

    if (!SDL_RenderTexture(this->pRenderer, this->pLightmapTex, nullptr, nullptr))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[Lightmap] ERROR: Texture cannot be rendered; %s", SDL_GetError());
    }
}

void Lightmap::addLight(Light2D* light)
{
	this->lights.push_back(light);
    SDL_Log("[Lightmap] LOG: Light with name '%s' was added to the lightmap.", light->getOwner()->getName().c_str());
}

void Lightmap::removeLight(Light2D* light)
{
    SDL_Log("[Lightmap] LOG: Light with name '%s' was removed from the lightmap.", light->getOwner()->getName().c_str());
	this->lights.erase(std::remove(this->lights.begin(), this->lights.end(), light), this->lights.end());
}

void Lightmap::drawLightmap()
{
    if (!this->bUpdateLightmap) return;

    //SDL_DestroyTexture(this->pLightmapTex);
    //this->pLightmapTex = SDL_CreateTexture(
    //    this->pRenderer,
    //    SDL_PIXELFORMAT_RGBA8888,
    //    SDL_TEXTUREACCESS_TARGET,
    //    gameWidth,
    //    gameHeight
    //);

    if (!SDL_SetRenderTarget(this->pRenderer, this->pLightmapTex))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[Lightmap] ERROR: Render target was not set; %s", SDL_GetError());
    }

    SDL_SetRenderDrawColor(this->pRenderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(this->pRenderer);

    for (Light2D* light : lights) light->drawLight();

    SDL_SetRenderTarget(this->pRenderer, nullptr);
}

void Lightmap::setAmbientColor(SDL_Color color)
{
    this->color = color;
}

SDL_Color Lightmap::getAmbientColor() const
{
    return this->color;
}

Lightmap* Lightmap::P_SHARED_INSTANCE = NULL;

Lightmap::Lightmap() : ARenderer("Lightmap")
{
    this->pRenderer = RendererContext::getInstance()->getRenderer();
    this->pLightmapTex = SDL_CreateTexture(
        this->pRenderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,
        gameWidth,
        gameHeight
    );
}

void Lightmap::initialize()
{
    EmptyObject* pManagerObject = new EmptyObject("Lightmap");
    P_SHARED_INSTANCE = new Lightmap();
    pManagerObject->attachComponent(P_SHARED_INSTANCE);
    GameObjectManager::getInstance()->addObject(pManagerObject);
}

void Lightmap::destroy()
{
    SDL_DestroyTexture(P_SHARED_INSTANCE->pLightmapTex);
    delete P_SHARED_INSTANCE->pOwner;
}

Lightmap* Lightmap::getInstance()
{
    return P_SHARED_INSTANCE;
}
