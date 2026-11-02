#include "TextRenderer.h"
#include "CameraManager.h"
#include "Camera.h"
#include <algorithm>

TextRenderer::TextRenderer() :ARenderer("TextRenderer")
{
    this->strFontType = "lazy";
    this->nFontSize = 12;
    this->strText = "";

    this->pTexture = NULL;
    this->pRenderer = RendererContext::getInstance()->getRenderer();
    this->flipX = false;
    this->flipY = false;
    this->dAngle = 0.0;
    this->texSize = Vector2D(0.0f, 0.0f);
    this->pivot = Vector2D(0.5f, 0.5f);
    this->mColor = SDL_Color(255, 255, 255, 255);
    this->bCropEnabled = false;
    this->fScrollOffset = 0.f;
    this->fViewHeight = 0.f;

    //SDL_Point anchor = { texW / 2,texH / 2 };
    mDestRect.x = 0;
    mDestRect.y = 0;
    mDestRect.w = this->texSize.x;
    mDestRect.h = this->texSize.y;
}

TextRenderer::~TextRenderer()
{
    if (pTexture)
    {
        SDL_DestroyTexture(pTexture);
        pTexture = NULL;
    }
}
void TextRenderer::initialize()
{
    // the texture is created later by loadFromText, so there is nothing to report if it is missing here
    if (pTexture != NULL)
    {

        float fw, fh;
        if (SDL_GetTextureSize(pTexture, &fw, &fh))
        {
            this->texSize = Vector2D(fw, fh);
        }

        if (mDestRect.w <= 0) mDestRect.w = this->texSize.x;
        if (mDestRect.h <= 0) mDestRect.h = this->texSize.y;
    }
}
void TextRenderer::perform()
{
    AGameObject* owner = this->getOwner();
    Camera* pCam = CameraManager::getInstance()->getCurrentCamera();
    SDL_FRect srcRect = {};

    if (owner)
    {
        this->dAngle = -owner->getRot();

        Vector2D scale = owner->getScale();
        Vector2D size = this->texSize * scale;
        Vector2D pos = owner->getPos();
        pos -= size * this->pivot;

        mDestRect.x = pos.x;
        mDestRect.y = pos.y;
        mDestRect.w = size.x;
        mDestRect.h = size.y;


        if (!owner->getIsScreenObject())
        {
            mDestRect = pCam->worldToScreenRect(mDestRect);
        }

        // the parent lookup is only needed when cropping
        AGameObject* pParent = this->bCropEnabled ? owner->getParent() : NULL;
        SpriteRenderer* pSpriteRenderer = NULL;
        if (pParent)
        {
            pSpriteRenderer = (SpriteRenderer*)pParent->findComponentByName("SpriteRenderer");
        }

        if (pParent && pSpriteRenderer)
        {
            SDL_FRect parentRect = pSpriteRenderer->getRect();
            SDL_FRect croppedSrcRect;

            if (SDL_GetRectIntersectionFloat(&parentRect, &mDestRect, &croppedSrcRect))
            {
                // offset in screen (dest) pixels between the cropped area and the original dest rect
                float offsetX = croppedSrcRect.x - mDestRect.x;
                float offsetY = croppedSrcRect.y - mDestRect.y;

                // Set destination to the cropped rectangle (this is in screen coords)
                mDestRect = croppedSrcRect;

                // Map dest (screen) pixels back to texture (source) pixels using the owner's scale.
                // size = texSize * scale earlier, so 1 source-pixel = scale pixels on screen.
                Vector2D scale = owner->getScale();

                // avoid division by zero
                float invScaleX = (scale.x != 0.0f) ? (1.0f / scale.x) : 1.0f;
                float invScaleY = (scale.y != 0.0f) ? (1.0f / scale.y) : 1.0f;

                // compute source rect in texture pixel coordinates
                srcRect.x = offsetX * invScaleX;                         // how many texture pixels from left
                srcRect.y = this->fScrollOffset + (offsetY * invScaleY); // add scroll offset (texture pixels)
                srcRect.w = croppedSrcRect.w * invScaleX;                // width in texture pixels
                srcRect.h = croppedSrcRect.h * invScaleY;                // height in texture pixels

                // clamp source rect to valid texture bounds just in case
                if (srcRect.x < 0) srcRect.x = 0;
                if (srcRect.y < 0) srcRect.y = 0;
                if (srcRect.x + srcRect.w > texSize.x) srcRect.w = texSize.x - srcRect.x;
                if (srcRect.y + srcRect.h > texSize.y) srcRect.h = texSize.y - srcRect.y;
            }
        }
    }

    if (pTexture && this->inCameraView(mDestRect))
    {
        // GPU draw call: theoretical O(1), but expensive constant cost.
        SDL_SetTextureColorMod(pTexture, mColor.r, mColor.g, mColor.b);
        SDL_SetTextureAlphaMod(pTexture, mColor.a);
        SDL_FRect* pSrcRect = NULL;
        if (srcRect.w != 0 && srcRect.h != 0)
        {
            pSrcRect = &srcRect;
        }
        if (this->flipX && this->flipY) SDL_RenderTextureRotated(pRenderer, pTexture, pSrcRect, &mDestRect, this->dAngle - 180.0f, NULL, SDL_FLIP_NONE);
        else if (this->flipX) SDL_RenderTextureRotated(pRenderer, pTexture, pSrcRect, &mDestRect, this->dAngle, NULL, SDL_FLIP_HORIZONTAL);
        else if (this->flipY) SDL_RenderTextureRotated(pRenderer, pTexture, pSrcRect, &mDestRect, this->dAngle, NULL, SDL_FLIP_VERTICAL);
        else SDL_RenderTextureRotated(pRenderer, pTexture, pSrcRect, &mDestRect, this->dAngle, NULL, SDL_FLIP_NONE);
    }
}
void TextRenderer::drawWidget()
{

}

void TextRenderer::loadFromText(std::string fontType, int nFontSize, std::string strText, SDL_Color textColor)
{
    this->strFontType = fontType;
    this->nFontSize = nFontSize;
    this->strText = strText;

    // rasterized in white, the renderer color tints it in perform so the color is not applied twice
    this->setColor(textColor);
    SDL_Color white = { 255, 255, 255, 255 };
    SDL_Texture* newTex = FontManager::getInstance()->createTextTexture(this->pRenderer, fontType, nFontSize, strText, white);

    // replace old texture if any
    if (this->pTexture)
    {
        SDL_DestroyTexture(this->pTexture);
        this->pTexture = nullptr;
    }

    if (!newTex)
    {
        this->texSize = Vector2D(0.0f, 0.0f);
        return;
    }

    this->pTexture = newTex;

    float fw = 0.0f, fh = 0.0f;
    if (SDL_GetTextureSize(pTexture, &fw, &fh))
    {
        this->texSize = Vector2D(fw, fh);
    }

    if (mDestRect.w <= 0) mDestRect.w = this->texSize.x;
    if (mDestRect.h <= 0) mDestRect.h = this->texSize.y;
}

void TextRenderer::resetText()
{
    if (this->pTexture == NULL) return;
    SDL_DestroyTexture(this->pTexture);
    this->pTexture = nullptr;
}

void TextRenderer::onAttach()
{
    this->initialize();
}
float TextRenderer::getTextHeight()
{
    return this->mDestRect.h;
}
void TextRenderer::setCropEnabled(bool bCropEnabled)
{
    this->bCropEnabled = bCropEnabled;
}

void TextRenderer::setPivot(Vector2D pivot)
{
    this->pivot = Vector2D(SDL_clamp(pivot.x, 0, 1), SDL_clamp(pivot.y, 0, 1));
}

float TextRenderer::getScrollOffset()
{
    return this->fScrollOffset;
}

void TextRenderer::setScrollOffset(float fOffset)
{
    float maxOffset = std::max(0.0f, texSize.y - fViewHeight);
    fScrollOffset = SDL_clamp(fOffset, 0.0f, maxOffset);

}

void TextRenderer::setViewHeight(float fViewHeight)
{
    this->fViewHeight = fViewHeight;
}

float TextRenderer::getViewHeight()
{
    return this->fViewHeight;
}