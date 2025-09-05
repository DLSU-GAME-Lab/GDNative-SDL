#include "UIButton.h"

UIButton::UIButton(const std::string& strName, int nX,int nY, int nW, int nH):AGameObject(strName),nX(nX),nY(nY), 
nW(nW),nH(nH)
{
}

UIButton::~UIButton()
{
}

void UIButton::initialize()
{
	SpriteRenderer* pSpriteRenderer = new SpriteRenderer("button.png", nX, nY, nW,nH);
	SpriteRendererSystem::getInstance()->registerSpriteRenderer(pSpriteRenderer);
	this->attachComponent((AComponent*)pSpriteRenderer);

}
