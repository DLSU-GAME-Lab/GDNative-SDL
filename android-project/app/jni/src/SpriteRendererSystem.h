#pragma once

#include <SDL3/SDL.h>
#include <vector>

class SpriteRenderer;

class SpriteRendererSystem
{
private:
    std::vector<SpriteRenderer*> vecSpriteRenderers;
    SDL_Renderer* pRenderer = NULL;

public:
    void draw();
    void registerSpriteRenderer(SpriteRenderer* pSpriteRenderer);
    void unregisterSpriteRenderer(SpriteRenderer* pSpriteRenderer);
public:
    SDL_Renderer* getRenderer();

    /* * * * * * * * * * * * * * * * * * * * *
     *       SINGLETON-RELATED CONTENT       *
     * * * * * * * * * * * * * * * * * * * * */
private:
    static SpriteRendererSystem* P_SHARED_INSTANCE;

private:
    SpriteRendererSystem() {};
    SpriteRendererSystem(const SpriteRendererSystem&) {};
    SpriteRendererSystem& operator=(const SpriteRendererSystem&) {};

public:
    static void initialize(SDL_Renderer* pRenderer);
    static void destroy();

    static SpriteRendererSystem* getInstance();
    /* * * * * * * * * * * * * * * * * * * * */
};

