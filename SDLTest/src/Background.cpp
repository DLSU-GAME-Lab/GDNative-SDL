#include "Background.h"
#include "SpriteRenderer.h"
#include "Settings.h"

Background::Background(const std::string& strName, const std::string& strImageName,Vector2D fVecScale)
    : AGameObject(strName), strImageName(strImageName)
{
    this->fVecScale = fVecScale;
    this->bIsScreenObject = true;
}

void Background::initialize()
{
    SpriteRenderer* pSpriteRenderer = new SpriteRenderer(strImageName);
    this->attachComponent((AComponent*)pSpriteRenderer);
    pSpriteRenderer->setPivot(Vector2D(0.0f, 0.0f));
}
