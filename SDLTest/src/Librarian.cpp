#include "Librarian.h"
#include "TextureManager.h"
#include "SpriteAnimator.h"
#include "ButtonInput.h"
#include "GUIToggle.h"

Librarian::Librarian(Vector2D fVecTranslate, Vector2D fVecScale, float fRot):AGameObject("Librarian")
{
    this->fVecTranslate = fVecTranslate;
    this->fVecScale = fVecScale;
}

void Librarian::initialize()
{
    //1000, 350
    SpriteRenderer* pSpriteRenderer = new SpriteRenderer("Librarian");
    pSpriteRenderer->setCropRect({ 0.25f, 0.2f, 0.5f, 0.8f });
    this->attachComponent((AComponent*)pSpriteRenderer);

    auto vecSprite = TextureManager::getInstance()->getTexture("Librarian", 0, 6);
    SpriteAnimator* pSpriteAnimator = new SpriteAnimator(pSpriteRenderer, vecSprite, 8);
    this->attachComponent((AComponent*)pSpriteAnimator);

    ButtonInput* pInput = new ButtonInput(pSpriteRenderer);
    this->attachComponent(pInput);
    
    GUIToggle* pToggle = new GUIToggle(EventKey::DIALOGUE_SCREEN);
    this->attachComponent(pToggle);


    pSpriteRenderer->setFlipX(true);
}
