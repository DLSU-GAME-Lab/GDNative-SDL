// ---------------------------------------------------------------------------
// Responsibilities: per-object transform and texture draw.
// ---------------------------------------------------------------------------

#include <iostream>
#include "SpriteRenderer.h"
#include "TextureManager.h"
#include "AGameObject.h"
#include "Settings.h"

// Constructor: texture lookup may be O(T) where T is number of
// textures stored under a name (small in typical cases). Overall O(1) prep.
SpriteRenderer::SpriteRenderer(const std::string& textureName, SDL_Color color)
    : ARenderer("SpriteRenderer"), pTexture(nullptr), m_textureKey(textureName)
{
    // Constructor setup: mostly O(1) except for texture lookup.
    this->flipX = false;
    this->flipY = false;
    this->dAngle = 0.0;
    this->texSize = Vector2D(0.0f, 0.0f);
    this->pivot = Vector2D(0.5f, 0.5f);
    this->mColor = color;

    auto textures = TextureManager::getInstance()->getTexture(textureName);

    if (!textures.empty()) {
        pTexture = textures[0]; // O(1) fetch from vector
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
    mDestRect.x = 0;
    mDestRect.y = 0;
    mDestRect.w = this->texSize.x;
    mDestRect.h = this->texSize.y;

    //mDestRect.x = anchor.x - (mDestRect.w / 2);
    //mDestRect.y = anchor.y - (mDestRect.h / 2);
}

// Initialize: texture lookup may be O(T) where T is number of
// textures stored under a name (small in typical cases). Overall O(1) prep.
void SpriteRenderer::initialize() {
    // O(T): retrieves texture, updates size, registers sprite.
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

}

// Destructor: deregisters from RenderSystem — deregistration cost is O(S).
SpriteRenderer::~SpriteRenderer() {
    // O(R): deregistration scans list in RenderSystem.
    // unregister when destroyed

}

// perform: per-sprite O(1) math and single GPU draw call. As number of sprites R
// increases, total cost per-frame increases linearly (O(R)). GPU cost per
// perform is a significant constant-time cost in wall-time.
void SpriteRenderer::perform() {
    // O(1): All operations are per-sprite math and rendering.
    // Real runtime cost dominated by GPU draw call.
    AGameObject* owner = this->getOwner();
    Camera* pCam = CameraManager::getInstance()->getCurrentCamera();
    if (owner)
    {
        //TODO: fix the rotations. better if we used a transform matrix.
        this->dAngle = owner->getRot();

        Vector2D scale = owner->getScale();
        Vector2D size = this->texSize * scale;
        Vector2D pos = owner->getPos();
        pos -= size * this->pivot;

        mDestRect.x = pos.x;
        mDestRect.y = pos.y;
        mDestRect.w = size.x;
        mDestRect.h = size.y;

        if (!owner->getIsScreenObject()) mDestRect = pCam->worldToScreenRect(mDestRect);
    }

    // GPU draw call: theoretical O(1), but expensive constant cost.
    if (pTexture) {
        SDL_SetTextureColorMod(pTexture, mColor.r, mColor.g, mColor.b);
        SDL_SetTextureAlphaMod(pTexture, mColor.a);

        if (this->flipX && this->flipY) SDL_RenderTextureRotated(pRenderer, pTexture, NULL, &mDestRect, this->dAngle + 180.0f, NULL, SDL_FLIP_NONE);
        else if (this->flipX) SDL_RenderTextureRotated(pRenderer, pTexture, NULL, &mDestRect, this->dAngle, NULL, SDL_FLIP_HORIZONTAL);
        else if (this->flipY) SDL_RenderTextureRotated(pRenderer, pTexture, NULL, &mDestRect, this->dAngle, NULL, SDL_FLIP_VERTICAL);
        else SDL_RenderTextureRotated(pRenderer, pTexture, NULL, &mDestRect, this->dAngle, NULL, SDL_FLIP_NONE);
    }

    // additional log
    else if (SDL_RenderTexture(pRenderer, pTexture, nullptr, &mDestRect) < 0)
    {
        SDL_Log("SDL_RenderTexture failed: %s", SDL_GetError());
    }
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

void SpriteRenderer::setColor(SDL_Color color)
{
    this->mColor = color;
}

SDL_Texture* SpriteRenderer::getTexture()
{
    return this->pTexture;
}

SDL_Color SpriteRenderer::getColor() const
{
    return this->mColor;
}

SDL_FRect SpriteRenderer::getRect() const
{
    return this->mDestRect;
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

