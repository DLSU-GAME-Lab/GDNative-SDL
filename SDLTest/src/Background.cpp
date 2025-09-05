#include "Background.h"
#include "TextureManager.h"
#include "SpriteRenderer.h"

Background::Background(const std::string& strName, const std::string& strImageName)
    : AGameObject(strName), strImageName(strImageName)
{
    // create & attach sprite renderer
    // sample
    // this->addSpriteRenderer(strImageName, renderer, 0.0f, 0.0f);
    // SpriteRenderer* sprite = this->addSpriteRenderer(strImageName, renderer, 0.0f, 0.0f);

    //// Move the sprite somewhere else
    //sprite->setPosition(300.0f, 400.0f);

    // Resize the sprite to 128x128 (regardless of the original texture size)
    //sprite->setSize(128.0f, 128.0f);
}

Background::~Background()
{

}

void Background::initialize()
{

}
