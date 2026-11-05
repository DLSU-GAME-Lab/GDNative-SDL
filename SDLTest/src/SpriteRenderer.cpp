// ---------------------------------------------------------------------------
// Responsibilities: per-object transform and texture draw.
// Identical in Babaylan Tales and Gem Hunter Match.
// ---------------------------------------------------------------------------

#include <iostream>
#include <cmath>
#include "SpriteRenderer.h"
#include "TextureManager.h"
#include "RendererContext.h"
#include "AGameObject.h"
#include "Settings.h"
#include "ShaderManager.h"

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
    this->bShimmer = false;

    auto textures = TextureManager::getInstance()->getTexture(textureName);

    if (!textures.empty())
    {
        pTexture = textures[0]; // O(1) fetch from vector
    }
    else
    {
        std::cerr << "[ERROR] : Texture not found: " << textureName << std::endl;
    }


    if (pTexture)
    {
        float fw, fh;
        if (SDL_GetTextureSize(pTexture, &fw, &fh))
        {
            this->texSize = Vector2D(fw, fh);
        }
    }
    else
    {
        this->texSize = Vector2D(0.0f, 0.0f);
    }

    this->mCropRect = { 0, 0, 1, 1 };

    mDestRect.x = 0;
    mDestRect.y = 0;
    mDestRect.w = this->texSize.x;
    mDestRect.h = this->texSize.y;
}

// Initialize: texture lookup may be O(T) where T is number of
// textures stored under a name (small in typical cases). Overall O(1) prep.
void SpriteRenderer::initialize()
{
    // O(T): retrieves texture, updates size, registers sprite.
    auto textures = TextureManager::getInstance()->getTexture(m_textureKey);
    if (!textures.empty())
    {
        pTexture = textures[0];

        float fw, fh;
        if (SDL_GetTextureSize(pTexture, &fw, &fh))
        {
            this->texSize = Vector2D(fw, fh);
        }

        if (mDestRect.w <= 0) mDestRect.w = this->texSize.x;
        if (mDestRect.h <= 0) mDestRect.h = this->texSize.y;

        std::cout << "[SpriteRenderer] Initialized with texture: " << m_textureKey << std::endl;
    }
    else
    {
        std::cerr << "[SpriteRenderer ERROR] Texture not found during initialize: "
            << m_textureKey << std::endl;
    }

}

SpriteRenderer::~SpriteRenderer()
{}

// perform: per-sprite O(1) math and a single draw call. Total cost per frame
// grows linearly with the number of sprites.
void SpriteRenderer::perform()
{
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

    if (!pTexture || !this->inCameraView(mDestRect))
        return;

    // A sprite with a normal map always runs through the shader so it stays lit, the
    // sweep is added only while it is active (phase -1 = no sweep). Without a normal
    // map the shader runs only during a sweep. Without shader support the sprite is
    // drawn plain.
    ShaderMaterial* pMat = nullptr;
    float fPhase = -1.0f;
    if (bShimmer && !bNineSlice)
    {
        bool bSweeping = this->getShimmerPhase(fPhase);
        if (!bSweeping) fPhase = -1.0f;

        if (bSweeping || !m_normalMapKey.empty())
        {
            ShaderManager* pShaders = ShaderManager::getInstance();
            if (pShaders && pShaders->isSupported())
                pMat = pShaders->getShimmerMaterial(m_normalMapKey, m_maskMapKey);
        }
    }

    if (pMat)
    {
        ShimmerUniforms u = m_shimmerStyle;
        u.phase = fPhase;
        u.flipX = this->flipX ? -1.0f : 1.0f;
        u.flipY = this->flipY ? -1.0f : 1.0f;
        pMat->setShimmerUniforms(u);

        pMat->bind(pRenderer);
        drawSprite(srcRect);
        ShaderMaterial::unbind(pRenderer);
    }
    else
    {
        drawSprite(srcRect);
    }
}

// Draws through RendererContext so colour, flips and rotation behave exactly as
// they did before. Takes srcRect by value so we can hand RendererContext a
// pointer regardless of its const-ness.
void SpriteRenderer::drawSprite(SDL_FRect srcRect)
{
    RendererContext* pCtx = RendererContext::getInstance();

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
                pCtx->draw(this->pTexture, this->mColor, &src, &dst, 0.0f, false, false);
            }
    }
    else
    {
        pCtx->draw(
            this->pTexture,
            this->mColor,
            &srcRect,
            &mDestRect,
            (float)this->dAngle,
            this->flipX,
            this->flipY
        );
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

// Looks the texture up by key so initialize() keeps the new texture too, and
// refreshes texSize because perform() builds the source rect from it.
void SpriteRenderer::setTextureKey(const std::string& textureKey)
{
    auto textures = TextureManager::getInstance()->getTexture(textureKey);
    if (textures.empty())
    {
        std::cerr << "[SpriteRenderer ERROR] Texture not found: " << textureKey << std::endl;
        return;
    }

    this->m_textureKey = textureKey;
    this->pTexture = textures[0];

    float fw, fh;
    if (SDL_GetTextureSize(this->pTexture, &fw, &fh))
    {
        this->texSize = Vector2D(fw, fh);
    }
}

void SpriteRenderer::setPosition(float x, float y)
{
    mDestRect.x = x;
    mDestRect.y = y;
}

void SpriteRenderer::setSize(float w, float h)
{
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

void SpriteRenderer::setShimmer(bool bEnabled, const std::string& normalMapKey, const std::string& maskKey)
{
    this->bShimmer = bEnabled;
    this->m_normalMapKey = normalMapKey;
    this->m_maskMapKey = maskKey;
}

void SpriteRenderer::setShimmerTiming(float fSpeed, float fCooldown, float fOffset)
{
    this->m_fShimmerSpeed = fSpeed;
    this->m_fShimmerCooldown = fCooldown;
    this->m_fShimmerOffset = fOffset;
}

// Sweep timing comes from the clock, so no component is needed. Every cycle is one
// sweep (1 / speed seconds) followed by the cooldown. Returns false while resting.
bool SpriteRenderer::getShimmerPhase(float& fPhase) const
{
    if (m_fShimmerSpeed <= 0.0f) return false;

    double dSweep = 1.0 / m_fShimmerSpeed;
    double dCycle = dSweep + m_fShimmerCooldown;
    double dTime = std::fmod((double)SDL_GetTicks() / 1000.0 + m_fShimmerOffset, dCycle);
    if (dTime >= dSweep) return false;

    fPhase = (float)(dTime / dSweep);
    return true;
}

void SpriteRenderer::setShimmerStyle(const ShimmerUniforms& style)
{
    this->m_shimmerStyle = style;
}


SDL_Texture* SpriteRenderer::getTexture()
{
    return this->pTexture;
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