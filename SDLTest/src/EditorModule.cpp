#include "EditorModule.h"
#include "TextureManager.h"
#include "GameObjectManager.h"
#include "UIManager.h"

void EditorModule::processEditorInput(const SDL_Event* eEvent)
{
    UIManager::getInstance()->processEvent(eEvent);

    std::vector<AGameObject*> vecObject = GameObjectManager::getInstance()->getAllObjects();
    for (auto object : vecObject)
    {
        if (this->contains(object->getPos(), eEvent->motion.x, eEvent->motion.y))
        {
            switch (eEvent->type)
            {
            case SDL_EVENT_MOUSE_MOTION:
                //this->OnHovered();
                break;

            case SDL_EVENT_MOUSE_BUTTON_DOWN:
                //this->OnPressed(eEvent->button);
                break;

            case SDL_EVENT_MOUSE_BUTTON_UP:
                //this->OnReleased(eEvent->button);
                break;
            }
        }
    }
}

void EditorModule::updateGameObjects()
{
    std::vector<AGameObject*> vecObject = GameObjectManager::getInstance()->getAllObjects();
    for (auto object : vecObject)
    {

    }
}

void EditorModule::drawEditor(SDL_Renderer* pRenderer)
{
    std::vector<AGameObject*> vecObject = GameObjectManager::getInstance()->getAllObjects();
    for (auto object : vecObject)
    {
        SDL_FRect mDestRect {};
        mDestRect.x = object->getPos().x;
        mDestRect.y = object->getPos().y;
        mDestRect.w = this->fTexW;
        mDestRect.h = this->fTexH;

        SDL_RenderTexture(pRenderer, this->pWidget, NULL, &mDestRect);
    }

    UIManager::getInstance()->newFrame();
    UIManager::getInstance()->drawAllUI(pRenderer);

    ImGuiIO io = ImGui::GetIO();
    SDL_SetRenderScale(pRenderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
}

bool EditorModule::contains(Vector2D objPos, float fX, float fY)
{
    if (!this->pWidget) return false;

    SDL_FRect spriteRect = {objPos.x, objPos.y, this->fTexW, this->fTexH};
    SDL_FRect pointRect = { fX, fY, 1, 1 };

    return SDL_HasRectIntersectionFloat(&spriteRect, &pointRect);
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
