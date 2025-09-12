#include "Background.h"
#include "SpriteRendererSystem.h"
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
    SpriteRenderer* pSpriteRenderer = new SpriteRenderer(strImageName, 0, 0, 0, gameWidth, gameHeight);
    SpriteRendererSystem::getInstance()->registerSpriteRenderer(pSpriteRenderer);
    this->attachComponent((AComponent*)pSpriteRenderer);
}
