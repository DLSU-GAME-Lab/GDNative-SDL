#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include "Vector2D.h"

class EditorModule
{
private:
    SDL_Texture* pWidget = NULL;
    float fTexW = 64.0f;
    float fTexH = 64.0f;

public:
    void processEditorInput(const SDL_Event* eEvent);
    void updateGameObjects();
    void drawEditor(SDL_Renderer* pRenderer);

private:
    bool contains(Vector2D objPos, float fX, float fY);

    /* * * * * * * * * * * * * * * * * * * * *
     *       SINGLETON-RELATED CONTENT       *
     * * * * * * * * * * * * * * * * * * * * */
private:
    static EditorModule* P_SHARED_INSTANCE;

private:
    EditorModule();
    EditorModule(const EditorModule&) {};
    EditorModule& operator = (const EditorModule&) {};

public:
    static void initialize(SDL_Window* window, SDL_Renderer* renderer);
    static void destroy();

    static EditorModule* getInstance();
    /* * * * * * * * * * * * * * * * * * * * */
};

