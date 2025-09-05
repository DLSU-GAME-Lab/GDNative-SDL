#include "UIButton.h"

UIButton::UIButton(const std::string& strName, int nX,int nY, int nW, int nH, bool bFlipX):AGameObject(strName),nX(nX),nY(nY), 
nW(nW),nH(nH), bFlipX(bFlipX)
{
}

UIButton::~UIButton()
{
}

void UIButton::initialize()
{
	SpriteRenderer* pSpriteRenderer = new SpriteRenderer("button.png", nX, nY, nW,nH);
	if (bFlipX)
	{
		pSpriteRenderer->setFlipX(true);
	}
	SpriteRendererSystem::getInstance()->registerSpriteRenderer(pSpriteRenderer);
	this->attachComponent((AComponent*)pSpriteRenderer);

}
