#include <iostream>
#include "SpriteRenderer.h"
#include "TextureManager.h"
#include "RenderSystem.h"
#include "AGameObject.h"
#include "Settings.h"

SpriteRenderer::SpriteRenderer(const std::string& textureName, float x, float y, float w, float h)
    : AComponent("SpriteRenderer", ComponentType::RENDERER), pTexture(nullptr), m_textureKey(textureName)
{
    this->flipX = false;
    this->flipY = false;
    this->dAngle = 0.0;
    this->texSize = Vector2D(0.0f, 0.0f);
    this->pivot = Vector2D(0.5f, 0.5f);

    auto textures = TextureManager::getInstance()->getTexture(textureName);

    if (!textures.empty()) {
        pTexture = textures[0];
    }
    else {
        std::cerr << "[ERROR] : Texture not found: " << textureName << std::endl;
    }

   
    if (pTexture) {
        float fw, fh;
        if (SDL_GetTextureSize(pTexture, &fw, &fh)) {
            this->texSize = Vector2D(fw, fh);
        }
    }
    else {
        this->texSize = Vector2D(0.0f, 0.0f);
    }

    //SDL_Point anchor = { texW / 2,texH / 2 };
    mDestRect.x = x;
    mDestRect.y = y;
    mDestRect.w = (w > 0) ? w : this->texSize.x;
    mDestRect.h = (h > 0) ? h : this->texSize.y;

    //mDestRect.x = anchor.x - (mDestRect.w / 2);
    //mDestRect.y = anchor.y - (mDestRect.h / 2);
}

void SpriteRenderer::initialize() {
    auto textures = TextureManager::getInstance()->getTexture(m_textureKey);
    if (!textures.empty()) {
        pTexture = textures[0];

        float fw, fh;
        if (SDL_GetTextureSize(pTexture, &fw, &fh)) {
            this->texSize = Vector2D(fw, fh);
        }

        if (mDestRect.w <= 0) mDestRect.w = this->texSize.x;
        if (mDestRect.h <= 0) mDestRect.h = this->texSize.y;

        std::cout << "[SpriteRenderer] Initialized with texture: " << m_textureKey << std::endl;
    }
    else {
        std::cerr << "[SpriteRenderer ERROR] Texture not found during initialize: "
            << m_textureKey << std::endl;
    }

    // register this sprite with the system
    RenderSystem::getInstance()->registerSpriteRenderer(this);
}

SpriteRenderer::~SpriteRenderer() {
    // unregister when destroyed
    RenderSystem::getInstance()->unregisterSpriteRenderer(this);
}

void SpriteRenderer::draw(SDL_Renderer* pRenderer, Camera* pCam) {
    AGameObject* owner = this->getOwner();
    if (owner)
    {
        //TODO: fix the rotations. better if we used a transform matrix.
        this->dAngle = owner->getRot();

        Vector2D screenPos;
        Vector2D screenSize = this->texSize * owner->getScale();

        if (owner->getIsScreenObject())
        {
            screenPos = owner->getPos() + screenSize;
        }
        else
        {
            screenSize /= pCam->getScale();
            screenPos = pCam->worldToScreenPoint(owner->getPos());
            this->dAngle -= pCam->getRot();
        }

        screenPos -= screenSize * this->pivot;
        //TODO: Fix the negative rect error (screenPos)
        mDestRect.x = screenPos.x;
        mDestRect.y = screenPos.y;
        mDestRect.w = screenSize.x;
        mDestRect.h = screenSize.y;
    }

    if (pTexture) {
        /*std::cout << "[Draw] Texture=" << m_textureKey
            << " Pos(" << mDestRect.x << "," << mDestRect.y << ")"
            << " Size(" << mDestRect.w << "," << mDestRect.h << ")" << std::endl;
        */
        if (this->flipX && this->flipY) SDL_RenderTextureRotated(pRenderer, pTexture, NULL, &mDestRect, this->dAngle, NULL, SDL_FLIP_HORIZONTAL); //replace with both flipped when available
        if (this->flipX) SDL_RenderTextureRotated(pRenderer, pTexture, NULL, &mDestRect, this->dAngle, NULL, SDL_FLIP_HORIZONTAL);
        else if (this->flipY) SDL_RenderTextureRotated(pRenderer, pTexture, NULL, &mDestRect, this->dAngle, NULL, SDL_FLIP_VERTICAL);
        else SDL_RenderTextureRotated(pRenderer, pTexture, NULL, &mDestRect, this->dAngle, NULL, SDL_FLIP_NONE);
        //SDL_Log("SDL_RenderTexture failed: %s", SDL_GetError());
    }

    // additional log
    else if (SDL_RenderTexture(pRenderer, pTexture, nullptr, &mDestRect) < 0) {
        SDL_Log("SDL_RenderTexture failed: %s", SDL_GetError());
    }
}

void SpriteRenderer::perform()
{
}

void SpriteRenderer::setTexture(SDL_Texture* pTexture)
{
    this->pTexture = pTexture;
}

void SpriteRenderer::setPosition(float x, float y) {
    mDestRect.x = x;
    mDestRect.y = y;
}

void SpriteRenderer::setSize(float w, float h) {
    mDestRect.w = w;
    mDestRect.h = h;
}


void SpriteRenderer::setFlipX(bool flipX)
{
    this->flipX = flipX;
}

void SpriteRenderer::setFlipY(bool flipY)
{
    this->flipY = flipY;
}

void SpriteRenderer::setAngle(double dAngle)
{
    this->dAngle = dAngle;
}

void SpriteRenderer::setPivot(Vector2D pivot)
{
    this->pivot = Vector2D(SDL_clamp(pivot.x, 0, 1), SDL_clamp(pivot.y, 0, 1));
}

SDL_Texture* SpriteRenderer::getTexture()
{
    return this->pTexture;
}

bool SpriteRenderer::getflipX()
{
    return this->flipX;
}

bool SpriteRenderer::getFlipY()
{
    return this->flipY;
}

double SpriteRenderer::getAngle()
{
    return this->dAngle;
}

Vector2D SpriteRenderer::getPivot()
{
    return this->pivot;
}

