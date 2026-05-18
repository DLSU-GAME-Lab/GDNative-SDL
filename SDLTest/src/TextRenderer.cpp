#include "TextRenderer.h"

TextRenderer::TextRenderer():ARenderer("TextRenderer")
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

    if (pTexture != NULL) {

        float fw, fh;
        if (SDL_GetTextureSize(pTexture, &fw, &fh)) {
            this->texSize = Vector2D(fw, fh);
        }

        if (mDestRect.w <= 0) mDestRect.w = this->texSize.x;
        if (mDestRect.h <= 0) mDestRect.h = this->texSize.y;

        std::cout << "[Dialogue Renderer]: Dialogue Loaded" << std::endl;;
    }
    else {
        std::cerr << "[Dialogue Renderer ERROR] No Dialogue Texture to load: "<< std::endl;
    }
}
void TextRenderer::perform()
{
    AGameObject* owner = this->getOwner();
    Camera* pCam = CameraManager::getInstance()->getCurrentCamera();
    SDL_FRect srcRect = {};
    AGameObject* pParent = this->getOwner()->getParent();
    SpriteRenderer* pSpriteRenderer = NULL;
    if (pParent)
    {
        pSpriteRenderer = (SpriteRenderer*)pParent->findComponentByName("SpriteRenderer");
    }

    if (owner)
    {
        this->dAngle = -owner->getRot();

        Vector2D windowOffset = CameraManager::getInstance()->getWindowOffset();
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
        if (pParent && pSpriteRenderer && this->bCropEnabled)
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

    if (this->inCameraView(mDestRect))
    {
        // GPU draw call: theoretical O(1), but expensive constant cost.
        if (pTexture)
        {
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

        // additional log
        else if (SDL_RenderTexture(pRenderer, pTexture, nullptr, &mDestRect) < 0)
        {
            SDL_Log("SDL_RenderTexture failed: %s", SDL_GetError());
        }
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

    TTF_Font* font = FontManager::getInstance()->getFont(fontType, nFontSize);
    if (!font) {
        const char * err = nullptr;
#ifdef TTF_GetError
        err = TTF_GetError();
#else
        err = SDL_GetError();
#endif
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[TextRenderer] font is null for %s size=%d. Error: %s",
                     fontType.c_str(), nFontSize, err != nullptr ? err : "unknown");
        return;
    }

    if (strText.empty()) {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "[TextRenderer] empty text for %s", strName.c_str());
        if (this->pTexture) { SDL_DestroyTexture(this->pTexture); this->pTexture = nullptr; }
        return;
    }

    int wrapWidth = 0; // pixel width for wrapping; 0 = no wrapping (only newline)
    // note: signature requires length BEFORE color
    SDL_Surface* textSurface = TTF_RenderText_Blended_Wrapped(font, strText.c_str(), strText.size(), textColor, wrapWidth);
    if (!textSurface) {
        const char* err = nullptr;
#ifdef TTF_GetError
        err = TTF_GetError();
#else
        err = SDL_GetError();
#endif
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[TextRenderer] TTF_Render failed: %s", err ? err : "unknown");
        return;
    }

    // replace old texture if any
    if (this->pTexture) {
        SDL_DestroyTexture(this->pTexture);
        this->pTexture = nullptr;
    }

    SDL_Texture* newTex = SDL_CreateTextureFromSurface(this->pRenderer, textSurface);
    SDL_DestroySurface(textSurface); // use SDL_DestroySurface (SDL_FreeSurface alias is okay but caused macro confusion)

    if (!newTex) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[TextRenderer] CreateTextureFromSurface failed: %s", SDL_GetError());
        return;
    }

    this->pTexture = newTex;

    float fw = 0.0f, fh = 0.0f;
    if (SDL_GetTextureSize(pTexture, &fw, &fh)) {
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

void TextRenderer::setColor(SDL_Color color)
{
    ARenderer::setColor(color);
    this->loadFromText(this->strFontType, this->nFontSize, this->strText, color);
}
