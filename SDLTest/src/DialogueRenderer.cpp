#include "DialogueRenderer.h"

DialogueRenderer::DialogueRenderer():ARenderer("DialogueRenderer")
{
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

DialogueRenderer::~DialogueRenderer()
{
	if (pTexture)
	{
		SDL_DestroyTexture(pTexture);
		pTexture = NULL;
	}
}
void DialogueRenderer::initialize()
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
void DialogueRenderer::perform()
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
                srcRect.x = 0;
                srcRect.y = 0;
                srcRect.w = croppedSrcRect.w;
                srcRect.h = croppedSrcRect.h;

                float offsetX = croppedSrcRect.x - mDestRect.x;
                float offsetY = croppedSrcRect.y - mDestRect.y;
                mDestRect = croppedSrcRect;
                mDestRect.w -= offsetX;
                mDestRect.h -= offsetY;

                std::cout << "Self: " << mDestRect.x << ", " << mDestRect.y << ", " << mDestRect.w << ", " << mDestRect.h << std::endl;
                std::cout << "Parent: " << parentRect.x << ", " << parentRect.y << ", " << parentRect.w << ", " << parentRect.h << std::endl;
                std::cout << "Source: " << srcRect.x << ", " << srcRect.y << ", " << srcRect.w << ", " << srcRect.h << std::endl;
                std::cout << "Cropped Source: " << croppedSrcRect.x << ", " << croppedSrcRect.y << ", " << croppedSrcRect.w << ", " << croppedSrcRect.h << std::endl;

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
void DialogueRenderer::drawWidget()
{

}

void DialogueRenderer::loadFromText(std::string strName, std::string fontType, int nFontSize, std::string textureText, SDL_Color textColor)
{
    // O(1): creates texture from text; I/O and render cost from SDL_ttf.
    SDL_Surface* textSurface = TTF_RenderText_Blended_Wrapped(FontManager::getInstance()->getFont(fontType, nFontSize), textureText.c_str(), 0, textColor, 0);
    if (textSurface == nullptr)
    {
        std::cout << "[ERROR]: Could not render text." << SDL_GetError() << std::endl;
        return;
    }
    SDL_Texture* pTexture = SDL_CreateTextureFromSurface(this->pRenderer, textSurface);
    SDL_DestroySurface(textSurface);
    if (!pTexture) {
        std::cerr << "[ERROR] : Failed to create texture for [" << strName << "] "
            << "Error: " << SDL_GetError() << std::endl;
        return;
    }
    this->pTexture = pTexture;
    float fw, fh;
    if (SDL_GetTextureSize(pTexture, &fw, &fh)) {
        this->texSize = Vector2D(fw, fh);
    }

    if (mDestRect.w <= 0) mDestRect.w = this->texSize.x;
    if (mDestRect.h <= 0) mDestRect.h = this->texSize.y;
}

void DialogueRenderer::resetText()
{
    SDL_DestroyTexture(this->pTexture);
    this->pTexture = nullptr;
}

void DialogueRenderer::onAttach()
{
    this->initialize();
}
float DialogueRenderer::getTextHeight()
{
    return this->mDestRect.h;
}
void DialogueRenderer::setCropEnabled(bool bCropEnabled)
{
    this->bCropEnabled = bCropEnabled;
}

void DialogueRenderer::setPivot(Vector2D pivot)
{
    this->pivot = Vector2D(SDL_clamp(pivot.x, 0, 1), SDL_clamp(pivot.y, 0, 1));
}

float DialogueRenderer::getScrollOffset()
{
    return this->fScrollOffset;
}

void DialogueRenderer::setScrollOffset(float fOffset)
{
    float maxOffset = std::max(0.0f, texSize.y - fViewHeight);
    fScrollOffset = SDL_clamp(fOffset, 0.0f, maxOffset);

}

void DialogueRenderer::setViewHeight(float fViewHeight)
{
    this->fViewHeight = fViewHeight;
}

float DialogueRenderer::getViewHeight()
{
    return 0.0f;
}
