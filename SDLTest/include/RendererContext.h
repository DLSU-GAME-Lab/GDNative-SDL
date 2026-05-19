#pragma once
#include <SDL3/SDL.h>
#include "Vector2D.h"

class RendererContext
{
private:
    SDL_Renderer* pRenderer;
public:
    void draw(
        SDL_Texture* pTexture,
        SDL_Color color,
        Vector2D size,
        Vector2D pivot,
        Vector2D position,
        Vector2D scale,
        float rotation,
        SDL_FRect cropRect,
        bool flipX = false,
		bool flipY = false
	);
    void draw(
        SDL_Texture* pTexture,
        SDL_Color color,
        SDL_FRect* srcRect,
        SDL_FRect* dstRect,
		float rotation = 0.0f,
        bool flipX = false,
		bool flipY = false
	);

    SDL_Renderer* getRenderer();
    Vector2D getRenderScale() const;

/* * * * * * * * * * * * * * * * * * * * *
 *       SINGLETON-RELATED CONTENT       *
 * * * * * * * * * * * * * * * * * * * * */
private:
    static RendererContext* P_SHARED_INSTANCE;

private:
    RendererContext(SDL_Renderer* pRenderer);
    RendererContext(const RendererContext&) {};
    RendererContext& operator=(const RendererContext&) {};

public:
    static void initialize(SDL_Renderer* pRenderer);
    static void destroy();

    static RendererContext* getInstance();
    /* * * * * * * * * * * * * * * * * * * * */
};

