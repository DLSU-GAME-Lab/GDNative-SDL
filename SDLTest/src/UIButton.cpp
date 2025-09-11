#include "UIButton.h"
#include "SceneManager.h"
#include "SpriteRenderer.h"
#include <iostream>

UIButton::UIButton(std::string objName, std::string textureKey,
    Vector2D fVecTranslate, Vector2D fVecScale,
    float rotation, bool isUI, SceneTag eTargetScene)
    : AButton(objName), m_eTargetScene(eTargetScene), m_textureKey(textureKey)
{
    // explicitly use the texture key stored in TextureManager
    this->pSprite = new SpriteRenderer(m_textureKey, fVecTranslate.x, fVecTranslate.y);
    this->attachComponent(this->pSprite);

    this->fVecTranslate = fVecTranslate;
    this->fVecScale = fVecScale;
    this->pSprite->setAngle(rotation);

    this->pSprite = dynamic_cast<SpriteRenderer*>(this->findComponentByName("SpriteRenderer"));
}

void UIButton::initialize()
{
    this->pSprite->initialize();
    
    std::cout << "[UIButton] Initialized: " << this->getName()
        << " using texture: " << m_textureKey << std::endl;

    if (this->pSprite->getTexture() == nullptr) {
        std::cerr << "[UIButton ERROR] Texture not found for: " << m_textureKey << std::endl;
    }
}

void UIButton::OnPressed(SDL_MouseButtonEvent eMouseEvent)
{
    std::cout << "[UIButton] Button pressed: " << this->getName() << std::endl;
}

void UIButton::OnReleased(SDL_MouseButtonEvent eMouseEvent)
{
    std::cout << "[UIButton] Button released: " << this->getName() << std::endl;
    SceneManager::getInstance()->loadScene(m_eTargetScene); // fixed
}

void UIButton::OnHovered()
{
    std::cout << "[UIButton] Hovering over: " << this->getName() << std::endl;
}
