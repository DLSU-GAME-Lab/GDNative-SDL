#include "RendererContext.h"
#include "CameraManager.h"

// perform: per-sprite O(1) math and single GPU draw call. As number of sprites R
// increases, total cost per-frame increases linearly (O(R)). GPU cost per
// perform is a significant constant-time cost in wall-time.
void RendererContext::draw(
    SDL_Texture* pTexture,
    SDL_Color color,
    SDL_FRect* srcRect,
    SDL_FRect* dstRect,
    float rotation,
    bool flipX,
    bool flipY)
{
    // O(1): All operations are per-sprite math and rendering.
    // Real runtime cost dominated by GPU draw call.

    // GPU draw call: theoretical O(1), but expensive constant cost.
    if (pTexture)
    {
        SDL_SetTextureColorMod(pTexture, color.r, color.g, color.b);
        SDL_SetTextureAlphaMod(pTexture, color.a);

        if (flipX && flipY) SDL_RenderTextureRotated(pRenderer, pTexture, srcRect, dstRect, rotation - 180.0f, NULL, SDL_FLIP_NONE);
        else if (flipX) SDL_RenderTextureRotated(pRenderer, pTexture, srcRect, dstRect, rotation, NULL, SDL_FLIP_HORIZONTAL);
        else if (flipY) SDL_RenderTextureRotated(pRenderer, pTexture, srcRect, dstRect, rotation, NULL, SDL_FLIP_VERTICAL);
        else SDL_RenderTextureRotated(pRenderer, pTexture, srcRect, dstRect, rotation, NULL, SDL_FLIP_NONE);
    }

    // additional log
    else if (SDL_RenderTexture(pRenderer, pTexture, srcRect, dstRect) < 0)
    {
        SDL_Log("SDL_RenderTexture failed: %s", SDL_GetError());
    }
}

RendererContext* RendererContext::P_SHARED_INSTANCE = NULL;

RendererContext::RendererContext(SDL_Renderer* pRenderer) : pRenderer(pRenderer) {}

void RendererContext::initialize(SDL_Renderer* pRenderer)
{
    P_SHARED_INSTANCE = new RendererContext(pRenderer);
}
RendererContext* RendererContext::getInstance()
{
    return P_SHARED_INSTANCE;
}
void RendererContext::destroy()
{
    delete P_SHARED_INSTANCE;
}

SDL_Renderer* RendererContext::getRenderer()
{
    return this->pRenderer;
}

Vector2D RendererContext::getRenderScale() const
{
    float w, h;
    SDL_GetRenderScale(pRenderer, &w, &h);
    return Vector2D(w,h);
}
