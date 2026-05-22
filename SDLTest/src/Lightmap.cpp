#include "Lightmap.h"
#include "GameObjectManager.h"
#include "Settings.h"
#include "EmptyObject.h"
#include "RendererContext.h"

void Lightmap::drawWidget()
{
    for (Light2D* light : lights) if (light != nullptr) light->drawLightWidget();
}

void Lightmap::perform()
{
    drawLightmap();

    if (!SDL_RenderTexture(this->pRenderer, this->pLightmapTex, nullptr, nullptr))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[Lightmap] ERROR: Texture cannot be rendered; %s", SDL_GetError());
    }

    // For testing
    //for (Light2D* light : lights) light->drawLight();
}

void Lightmap::addLight(Light2D* pLight)
{
    this->lights.push_back(pLight);
    SDL_Log("[Lightmap] LOG: Light with name '%s' was added to the lightmap.", pLight->getOwner()->getName().c_str());
}

void Lightmap::removeLight(Light2D* pLight)
{
    if (std::find(this->lights.begin(), this->lights.end(), pLight) != this->lights.end())
    {
        SDL_Log("[Lightmap] LOG: Light with name '%s' was removed from the lightmap.", pLight->getOwner()->getName().c_str());
        this->lights.erase(std::remove(this->lights.begin(), this->lights.end(), pLight), this->lights.end());
    }
}

void Lightmap::drawLightmap()
{
    if (!this->bUpdateLightmap) return;

    if (bAccumulateLights)
    {
        // Pass 1: accumulate all lights onto black texture
        SDL_SetRenderTarget(this->pRenderer, this->pLightAccumTex);
        SDL_SetRenderDrawBlendMode(this->pRenderer, SDL_BLENDMODE_NONE);
        SDL_SetRenderDrawColor(this->pRenderer, 0, 0, 0, 255);
        SDL_RenderClear(this->pRenderer);

        for (Light2D* light : lights) if (light != nullptr) light->drawLight();

        if (!SDL_SetRenderTarget(this->pRenderer, this->pLightmapTex))
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[Lightmap] ERROR: Render target was not set; %s", SDL_GetError());
        }

        // Pass 2: combine ambient + accumulated lights onto lightmap
        SDL_SetRenderTarget(this->pRenderer, this->pLightmapTex);
        SDL_SetRenderDrawBlendMode(this->pRenderer, SDL_BLENDMODE_NONE);
        SDL_SetRenderDrawColor(this->pRenderer, color.r, color.g, color.b, color.a);
        SDL_RenderClear(this->pRenderer);

        // Add the accumulated lights on top of ambient
        SDL_SetTextureBlendMode(this->pLightAccumTex, SDL_BLENDMODE_ADD);
        SDL_RenderTexture(this->pRenderer, this->pLightAccumTex, nullptr, nullptr);

        SDL_SetRenderTarget(this->pRenderer, nullptr);
    }
    else
    {
        if (!SDL_SetRenderTarget(this->pRenderer, this->pLightmapTex))
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[Lightmap] ERROR: Render target was not set; %s", SDL_GetError());
        }

        SDL_SetRenderDrawColor(this->pRenderer, color.r, color.g, color.b, color.a);
        SDL_RenderClear(this->pRenderer);

        for (Light2D* light : lights) if (light != nullptr) light->drawLight();

        SDL_SetRenderTarget(this->pRenderer, nullptr);
    }
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

    SDL_BlendMode customBlend = SDL_ComposeCustomBlendMode(
        SDL_BLENDFACTOR_ZERO,                // srcColorFactor
        SDL_BLENDFACTOR_SRC_COLOR,           // dstColorFactor <- multiplies scene by lightmap
        SDL_BLENDOPERATION_ADD,              // colorOperation
        SDL_BLENDFACTOR_ONE,                 // srcAlphaFactor
        SDL_BLENDFACTOR_ONE_MINUS_SRC_ALPHA, // dstAlphaFactor
        SDL_BLENDOPERATION_ADD               // alphaOperation
    );

    // Lightmap texture will hold the final combined result of ambient + all lights
    this->pLightmapTex = SDL_CreateTexture(
        this->pRenderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,
        gameWidth,
        gameHeight
    );

    SDL_SetTextureBlendMode(this->pLightmapTex, customBlend);
    // Accumulation texture will hold the combined result of all lights (no ambient)
    this->pLightAccumTex = SDL_CreateTexture(
        this->pRenderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,
        gameWidth,
        gameHeight
    );

    SDL_SetTextureBlendMode(this->pLightAccumTex, SDL_BLENDMODE_ADD);

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
    SDL_DestroyTexture(P_SHARED_INSTANCE->pLightAccumTex);
    delete P_SHARED_INSTANCE->pOwner;
}

Lightmap* Lightmap::getInstance()
{
    return P_SHARED_INSTANCE;
}
