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
    this->bNineSlice = false;
	this->nBorder = 0;
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

    this->mCropRect = { 0, 0, 1, 1 };

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
    this->dAngle = -owner->getRot();

    SDL_FRect srcRect = {};
    srcRect.x = this->mCropRect.x * this->texSize.x;
    srcRect.y = this->mCropRect.y * this->texSize.y;
    srcRect.w = this->mCropRect.w * this->texSize.x;
    srcRect.h = this->mCropRect.h * this->texSize.y;

    Vector2D scale = owner->getScale();
    Vector2D size = Vector2D(srcRect.w, srcRect.h) * scale;
    Vector2D pos = owner->getPos();
    pos -= size * this->pivot;

    mDestRect.x = pos.x;
    mDestRect.y = pos.y;
    mDestRect.w = size.x;
    mDestRect.h = size.y;

    if (!owner->getIsScreenObject())
    {
        mDestRect = pCam->worldToScreenRect(mDestRect);
        this->dAngle += pCam->getRot();
    }

    if (this->inCameraView(mDestRect))
    {
        // GPU draw call: theoretical O(1), but expensive constant cost.
        if (pTexture)
        {
            SDL_SetTextureColorMod(pTexture, mColor.r, mColor.g, mColor.b);
            SDL_SetTextureAlphaMod(pTexture, mColor.a);

            if (bNineSlice)
            {
                int b = nBorder;
                float tw = texSize.x, th = texSize.y;
                float W = mDestRect.w, H = mDestRect.h;
                float x = mDestRect.x, y = mDestRect.y;

                float srcX[3] = { 0,        (float)b,      tw - b };
                float srcY[3] = { 0,        (float)b,      th - b };
                float srcW[3] = { (float)b, tw - 2.0f * b, (float)b };
                float srcH[3] = { (float)b, th - 2.0f * b, (float)b };

                float dstX[3] = { x,        x + b,         x + W - b };
                float dstY[3] = { y,        y + b,          y + H - b };
                float dstW[3] = { (float)b, W - 2.0f * b,  (float)b };
                float dstH[3] = { (float)b, H - 2.0f * b,  (float)b };

                for (int row = 0; row < 3; row++)
                    for (int col = 0; col < 3; col++)
                    {
                        SDL_FRect src = { srcX[col], srcY[row], srcW[col], srcH[row] };
                        SDL_FRect dst = { dstX[col], dstY[row], dstW[col], dstH[row] };
                        SDL_RenderTexture(pRenderer, pTexture, &src, &dst);
                    }
            }
            else
            {
                // your existing flip/rotate logic unchanged
                if (this->flipX && this->flipY) SDL_RenderTextureRotated(pRenderer, pTexture, &srcRect, &mDestRect, this->dAngle - 180.0f, NULL, SDL_FLIP_NONE);
                else if (this->flipX) SDL_RenderTextureRotated(pRenderer, pTexture, &srcRect, &mDestRect, this->dAngle, NULL, SDL_FLIP_HORIZONTAL);
                else if (this->flipY) SDL_RenderTextureRotated(pRenderer, pTexture, &srcRect, &mDestRect, this->dAngle, NULL, SDL_FLIP_VERTICAL);
                else SDL_RenderTextureRotated(pRenderer, pTexture, &srcRect, &mDestRect, this->dAngle, NULL, SDL_FLIP_NONE);
            }
        }

        // additional log
        else if (SDL_RenderTexture(pRenderer, pTexture, &srcRect, &mDestRect) < 0)
        {
            SDL_Log("SDL_RenderTexture failed: %s", SDL_GetError());
        }
    }
}

void SpriteRenderer::drawWidget()
{
    Camera* pCam = CameraManager::getInstance()->getCurrentCamera();
    AGameObject* owner = this->getOwner();

    SDL_SetRenderDrawBlendMode(this->pRenderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(pRenderer, 255, 255, 255, 127);
    SDL_RenderRect(pRenderer, &mDestRect);

    Vector2D pos = pCam->worldToScreenPoint(owner->getPos());
    SDL_FRect pivotRect = { pos.x - 4, pos.y - 4, 8, 8 };
    SDL_SetRenderDrawColor(pRenderer, 255, 255, 255, 255);
    SDL_RenderFillRect(pRenderer, &pivotRect);
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

//Set crop based on percentage (0.0f - 1.0f)
void SpriteRenderer::setCropRect(SDL_FRect mCropRect)
{
    this->mCropRect = mCropRect;
}

void SpriteRenderer::setNineSlice(bool bEnabled, int nBorder)
{
    this->bNineSlice = bEnabled;
	this->nBorder = nBorder;
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

SDL_FRect SpriteRenderer::getCropRect() const
{
    return this->mCropRect;
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

