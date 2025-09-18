#include "EditorModule.h"
#include "TextureManager.h"
#include "GameObjectManager.h"
#include "RenderSystem.h"
#include "UIManager.h"
#include "InspectorScreen.h"
#include "EngineTime.h"
#include "Camera.h"
#include <iostream>

void EditorModule::processEditorInput(const SDL_Event* eEvent)
{
    UIManager::getInstance()->processEvent(eEvent);

    //ImGuiIO io = ImGui::GetIO();
    //io.WantCaptureMouse = false;
    InspectorScreen* inspector = static_cast<InspectorScreen*>(
        UIManager::getInstance()->getUIScreen("INSPECTOR_SCREEN"));

    if (eEvent->type == SDL_EVENT_MOUSE_MOTION)
    {
        this->mousePos = Vector2D(eEvent->motion.x, eEvent->motion.y);
        inspector->setMousePos(this->mousePos);
    }

    if (eEvent->button.button == 1)
    {
        if (eEvent->type == SDL_EVENT_MOUSE_BUTTON_DOWN)
        {
            this->bIsHolding = true;

            std::vector<AGameObject*> vecObject = GameObjectManager::getInstance()->getAllObjects();
            for (auto obj : vecObject)
            {
                if (inspector->getSelectedObject() != obj &&
                    contains(obj->getPos(), this->mousePos))
                {
                    inspector->setSelectedObject(obj);
                }
            }
        }
        else if (eEvent->type == SDL_EVENT_MOUSE_BUTTON_UP)
        {
            this->dTicks = 0.0f;
            this->bIsHolding = false;
            this->bIsDragging = false;
        }
    }
}

void EditorModule::updateGameObjects()
{
    AGameObject* selected = static_cast<InspectorScreen*>(
        UIManager::getInstance()->getUIScreen("INSPECTOR_SCREEN"))->getSelectedObject();

    if (!selected) return;

    if (this->bIsHolding)
    {
        this->dTicks += EngineTime::getInstance()->getDeltaTime();
        if (this->dTicks >= D_MAX_DRAG_TICKS)
        {
            this->dTicks = 0.0;
            this->bIsDragging = true;
        }
    }

    if (this->bIsDragging &&
        contains(selected->getPos(), this->mousePos))
    {
        selected->setPos(this->getMouseWorldPos());
    }
}

void EditorModule::drawEditor(SDL_Renderer* pRenderer)
{
    std::vector<AGameObject*> vecObject = GameObjectManager::getInstance()->getAllObjects();
    Camera* pCam = RenderSystem::getInstance()->getCamera();

    for (auto obj : vecObject)
    {
        SDL_FRect mDestRect {};
        mDestRect.w = this->fTexW;
        mDestRect.h = this->fTexH;

        Vector2D screenPos = pCam->worldToScreenPoint(obj->getPos());
        mDestRect.x = screenPos.x - (mDestRect.w * 0.5f);
        mDestRect.y = screenPos.y - (mDestRect.h * 0.5f);

        SDL_RenderTexture(pRenderer, this->pWidget, NULL, &mDestRect);
    }

    UIManager::getInstance()->newFrame();
    UIManager::getInstance()->drawAllUI(pRenderer);

    ImGuiIO io = ImGui::GetIO();
    SDL_SetRenderScale(pRenderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
}

bool EditorModule::contains(Vector2D objPos, Vector2D mousePos)
{
    if (!this->pWidget) return false;

    Camera* cam = RenderSystem::getInstance()->getCamera();

    SDL_FRect pointRect = { mousePos.x, mousePos.y, 1, 1};
    SDL_FRect spriteRect = {};
    
    Vector2D objScreenPos = cam->worldToScreenPoint(objPos);
    std::cout << objPos << " " << objScreenPos << "\n";
    spriteRect.x = objScreenPos.x - (fTexW * 0.5f);
    spriteRect.y = objScreenPos.y - (fTexH * 0.5f);
    spriteRect.w = fTexW;
    spriteRect.h = fTexH;

    return SDL_HasRectIntersectionFloat(&spriteRect, &pointRect);
}

Vector2D EditorModule::getMouseWorldPos() const
{
    Camera* cam = RenderSystem::getInstance()->getCamera();
    Vector2D mouseWorldPos;

    mouseWorldPos.x = (this->mousePos.x + cam->getPos().x - cam->getHalfWidth()) * cam->getScale().x;
    mouseWorldPos.y = (-(this->mousePos.y - cam->getPos().y - cam->getHalfHeight())) * cam->getScale().y;

    return mouseWorldPos;
}

/* * * * * * * * * * * * * * * * * * * * *
 *       SINGLETON-RELATED CONTENT       *
 * * * * * * * * * * * * * * * * * * * * */
EditorModule::EditorModule()
{
    TextureManager::getInstance()->load("editor/game_object_widget.png", "._widget");
    this->pWidget = TextureManager::getInstance()->get("._widget");
    SDL_GetTextureSize(this->pWidget, &this->fTexW, &this->fTexH);
}

EditorModule* EditorModule::P_SHARED_INSTANCE = NULL;

void EditorModule::initialize(SDL_Window* window, SDL_Renderer* renderer)
{
    UIManager::initialize(window, renderer);
    P_SHARED_INSTANCE = new EditorModule();
}

void EditorModule::destroy()
{
    delete P_SHARED_INSTANCE;
}

EditorModule* EditorModule::getInstance()
{
    return P_SHARED_INSTANCE;
}
/* * * * * * * * * * * * * * * * * * * * */
