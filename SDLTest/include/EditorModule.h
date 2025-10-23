#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include "Vector2D.h"
#include "AUIScreen.h"
#include "AGameObject.h"

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
        bool bIsMovingCam = false;

        float fDeltaScroll = 0.0f;
        Vector2D lastMousePos;
        Vector2D mousePos;
        Vector2D offsetPos;

    public:
        void processEditorInput(const SDL_Event* eEvent);
        void updateGameObjects(float fDeltaTime);
        void drawEditor(SDL_Renderer* pRenderer);

    private:
        bool contains(AGameObject* pObject, Vector2D mousePos);
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
        static void initialize();
        static void destroy();

        static EditorModule* getInstance();
        /* * * * * * * * * * * * * * * * * * * * */
    };
}
