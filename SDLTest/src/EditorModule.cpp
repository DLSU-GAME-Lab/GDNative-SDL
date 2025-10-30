#include "EditorModule.h"
#include "TextureManager.h"
#include "GameObjectManager.h"
#include "UIManager.h"
#include "InspectorScreen.h"
#include "CameraManager.h"
#include <iostream>
#include "MetricsManager.h"

void Editor::EditorModule::processEditorInput(const SDL_Event* eEvent)
{
    InspectorScreen* inspector = static_cast<InspectorScreen*>(
        UIManager::getInstance()->getUIScreen("INSPECTOR_SCREEN"));

    this->fDeltaScroll = 0.0f;
    this->lastMousePos = this->mousePos;

    if (eEvent->type == SDL_EVENT_MOUSE_WHEEL)
    {
        this->fDeltaScroll = -eEvent->wheel.y;
    }

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
                if (contains(obj, this->mousePos))
                {
                    inspector->setSelectedObject(obj);
                    this->offsetPos = this->getMouseWorldPos() - obj->getPos();
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
    else if (eEvent->button.button == 3)
    {
        if (eEvent->type == SDL_EVENT_MOUSE_BUTTON_DOWN)
        {
            this->bIsMovingCam = true;
        }
        else if (eEvent->type == SDL_EVENT_MOUSE_BUTTON_UP)
        {
            this->bIsMovingCam = false;
        }
    }
}

void Editor::EditorModule::updateGameObjects(float fDeltaTime)
{
    Camera* cam = CameraManager::getInstance()->getCurrentCamera();
    InspectorScreen* inspector = static_cast<InspectorScreen*>(
        UIManager::getInstance()->getUIScreen("INSPECTOR_SCREEN"));

    if (this->bIsMovingCam)
    {
        Vector2D camPos = cam->getPos();
        Vector2D deltaMousePos = this->mousePos - this->lastMousePos;
        deltaMousePos.x *= -1;

        cam->setPos(camPos + (deltaMousePos * fDeltaTime * inspector->getCamMoveSpeed()));
    }

    if (this->fDeltaScroll != 0.0f)
    {
        Vector2D camScale = cam->getScale();
        camScale += this->fDeltaScroll * fDeltaTime * inspector->getCamScaleSpeed();

        if (camScale.x < 0.0f)
        {
            camScale.x = 0.0f;
            camScale.y = 0.0f;
        }

        cam->setScale(camScale);
    }

    AGameObject* selected = static_cast<InspectorScreen*>(
        UIManager::getInstance()->getUIScreen("INSPECTOR_SCREEN"))->getSelectedObject();

    if (!selected) return;

    if (this->bIsHolding)
    {
        this->dTicks += fDeltaTime;
        if (this->dTicks >= D_MAX_DRAG_TICKS)
        {
            this->dTicks = 0.0;
            this->bIsDragging = true;
        }
    }

    if (this->bIsDragging && contains(selected, this->mousePos))
    {
        if (selected->getIsScreenObject()) selected->setPos(this->mousePos);
        else selected->setPos(this->getMouseWorldPos());
    }
}

void Editor::EditorModule::drawEditor(SDL_Renderer* pRenderer)
{
    Camera* pCam = CameraManager::getInstance()->getCurrentCamera();

    //SDL_RenderLine(pRenderer, 0, 540, 1920, 540);

    std::vector<AGameObject*> vecObject = GameObjectManager::getInstance()->getAllObjects();
    for (auto obj : vecObject)
    {
        SDL_FRect mDestRect {};
        mDestRect.w = this->fTexW;
        mDestRect.h = this->fTexH;

        Vector2D screenPos;
        if (obj->getIsScreenObject()) screenPos = obj->getPos();
        else screenPos = pCam->worldToScreenPoint(obj->getPos());

        mDestRect.x = screenPos.x - (mDestRect.w * 0.5f);
        mDestRect.y = screenPos.y - (mDestRect.h * 0.5f);

        SDL_RenderTexture(pRenderer, this->pWidget, NULL, &mDestRect);
    }

    MetricsManager::getInstance()->drawGUI();
}

bool Editor::EditorModule::contains(AGameObject* pObject, Vector2D mousePos)
{
    if (!this->pWidget) return false;

    Camera* cam = CameraManager::getInstance()->getCurrentCamera();

    SDL_FRect pointRect = { mousePos.x, mousePos.y, 1, 1};
    SDL_FRect spriteRect = {};
    Vector2D objPos;

    if (pObject->getIsScreenObject()) objPos = pObject->getPos();
    else objPos = cam->worldToScreenPoint(pObject->getPos());
    //std::cout << objPos << " " << objScreenPos << "\n";

    spriteRect.x = objPos.x - (fTexW * 0.5f);
    spriteRect.y = objPos.y - (fTexH * 0.5f);
    spriteRect.w = fTexW;
    spriteRect.h = fTexH;

    return SDL_HasRectIntersectionFloat(&spriteRect, &pointRect);
}

Vector2D Editor::EditorModule::getMouseWorldPos() const
{
    Camera* cam = CameraManager::getInstance()->getCurrentCamera();
    Vector2D mouseWorldPos = cam->screenToWorldPoint(this->mousePos);

    return mouseWorldPos;
}

/* * * * * * * * * * * * * * * * * * * * *
 *       SINGLETON-RELATED CONTENT       *
 * * * * * * * * * * * * * * * * * * * * */
Editor::EditorModule::EditorModule()
{
    TextureManager::getInstance()->load("editor/game_object_widget.png", "._widget");
    this->pWidget = TextureManager::getInstance()->get("._widget");
    SDL_GetTextureSize(this->pWidget, &this->fTexW, &this->fTexH);
}

Editor::EditorModule* Editor::EditorModule::P_SHARED_INSTANCE = NULL;

void Editor::EditorModule::initialize()
{
    P_SHARED_INSTANCE = new EditorModule();
}

void Editor::EditorModule::destroy()
{
    delete P_SHARED_INSTANCE;
}

Editor::EditorModule* Editor::EditorModule::getInstance()
{
    return P_SHARED_INSTANCE;
}
/* * * * * * * * * * * * * * * * * * * * */
