#pragma once

#include <SDL3/SDL.h>
#include <vector>

class SpriteRenderer;
class Camera;

class RenderSystem
{
private:
    std::vector<SpriteRenderer*> vecSpriteRenderers;
    Camera* pCamera = NULL;

public:
    void updateWindowSize(SDL_Window* pWindow, SDL_Renderer* pRenderer);
    void draw(SDL_Renderer* pRenderer);
    void registerSpriteRenderer(SpriteRenderer* pSpriteRenderer);
    void unregisterSpriteRenderer(SpriteRenderer* pSpriteRenderer);

    Camera* getCamera();

    /* * * * * * * * * * * * * * * * * * * * *
     *       SINGLETON-RELATED CONTENT       *
     * * * * * * * * * * * * * * * * * * * * */
private:
    static RenderSystem* P_SHARED_INSTANCE;

private:
    SDL_Renderer* pRenderer = nullptr;
public:
    SDL_Renderer* getRenderer() const { return this->pRenderer; }

private:
    RenderSystem() {};
    RenderSystem(const RenderSystem&) {};
    RenderSystem& operator=(const RenderSystem&) {};

public:
    static void initialize();
    static void destroy();

    static RenderSystem* getInstance();
    /* * * * * * * * * * * * * * * * * * * * */
};

