#include "Background.h"
#include "RenderSystem.h"
#include "Settings.h"

Background::Background(const std::string& strName, const std::string& strImageName,Vector2D fVecScale)
    : AGameObject(strName), strImageName(strImageName)
{
    this->fVecScale = fVecScale;
}

Background::~Background()
{

}

void Background::initialize()
{
    SpriteRenderer* pSpriteRenderer = new SpriteRenderer(strImageName, 0, 0, gameWidth, gameHeight);
    RenderSystem::getInstance()->registerSpriteRenderer(pSpriteRenderer);
    this->attachComponent((AComponent*)pSpriteRenderer);
    pSpriteRenderer->setPivot(Vector2D(0.0f, 0.0f));
}
