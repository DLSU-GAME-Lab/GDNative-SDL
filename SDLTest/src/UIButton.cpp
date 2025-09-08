#include "UIButton.h"
#include <iostream>

UIButton::UIButton(const std::string& strName, int nX,int nY, int nW, int nH, bool bFlipX, SceneTag targetScene):AGameObject(strName),nX(nX),nY(nY),
nW(nW),nH(nH), bFlipX(bFlipX)
{
}

UIButton::~UIButton()
{
}

void UIButton::initialize()
{
	SpriteRenderer* pSpriteRenderer = new SpriteRenderer("Button", nX, nY, nW,nH);
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
        if (mouseX >= nX && mouseX <= nX + nW &&
            mouseY >= nY && mouseY <= nY + nH) {
            std::cout << "Button clicked! Switching to scene: "
                << static_cast<int>(targetScene) << std::endl;

            // switch scenes
            SceneManager::getInstance()->loadScene(targetScene);
        }
    }
}
