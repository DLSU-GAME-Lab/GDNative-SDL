#pragma once

#include <vector>

class SpriteRenderer;

class SpriteRendererSystem
{
private:
    std::vector<SpriteRenderer*> vecSpriteRenderers;

public:
    void draw();
    void registerSpriteRenderer(SpriteRenderer* pSpriteRenderer);
    void unregisterSpriteRenderer(SpriteRenderer* pSpriteRenderer);

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
    static void initialize();
    static void destroy();

    static SpriteRendererSystem* getInstance();
    /* * * * * * * * * * * * * * * * * * * * */
};

