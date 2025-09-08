#include "UIButton.h"
#include <iostream>

UIButton::UIButton(const std::string& strName, float fX, float fY, float fW, float fH, float fRot, bool bFlipX, 
    SceneTag targetScene):AGameObject(strName), bFlipX(bFlipX)
{
    this->fX = this->fPosX = fX;
    this->fY = this->fPosY = fY;
    this->fW = this->fScaleX = fW;
    this->fH = this->fScaleY = fH;
    this->fRot = fRot;
}

UIButton::~UIButton()
{
}

void UIButton::initialize()
{
	SpriteRenderer* pSpriteRenderer = new SpriteRenderer(this->strName, fPosX, fPosY);
	if (bFlipX)
	{
		pSpriteRenderer->setFlipX(true);
	}
	SpriteRendererSystem::getInstance()->registerSpriteRenderer(pSpriteRenderer);
	this->attachComponent((AComponent*)pSpriteRenderer);

}

void UIButton::processInput(SDL_Event eEvent)
{
    if (eEvent.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        int mouseX = eEvent.button.x;
        int mouseY = eEvent.button.y;

        // check if click is within button bounds
        if (mouseX >= fX && mouseX <= fX + fW &&
            mouseY >= fY && mouseY <= fY + fH) {
            std::cout << "Button clicked! Switching to scene: "
                << static_cast<int>(targetScene) << std::endl;

            // switch scenes
            SceneManager::getInstance()->loadScene(targetScene);
        }
    }
}
