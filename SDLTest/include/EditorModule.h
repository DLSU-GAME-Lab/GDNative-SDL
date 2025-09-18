#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include "Vector2D.h"
#include "AUIScreen.h"

namespace Editor
{

    class EditorModule
    {
    private:
        SDL_Texture* pWidget = NULL;
        float fTexW = 64.0f;
        float fTexH = 64.0f;

        const double D_MAX_DRAG_TICKS = 0.2;
        double dTicks = 0.0;
        bool bIsDragging = false;
        bool bIsHolding = false;
        Vector2D mousePos;

    public:
        void processEditorInput(const SDL_Event* eEvent);
        void updateGameObjects();
        void drawEditor(SDL_Renderer* pRenderer);

    private:
        bool contains(Vector2D objPos, Vector2D mousePos);
        Vector2D getMouseWorldPos() const;

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
}
