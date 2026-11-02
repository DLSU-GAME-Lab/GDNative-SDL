#pragma once

#include "ARenderer.h"
#include "Vector2D.h"
#include "CameraManager.h"
#include "ShaderManager.h"
#include <SDL3/SDL.h>
#include <string>

// Identical in Babaylan Tales and Gem Hunter Match.
// Tint colour (mColor, setColor, getColor) and inCameraView() come from ARenderer.
class SpriteRenderer : public ARenderer
{
private:
    SDL_Texture* pTexture;
    SDL_FRect mDestRect;
    SDL_FRect mCropRect;
    bool flipX, flipY;
    double dAngle;
    std::string m_textureKey;
    Vector2D texSize;
    Vector2D crop;
    Vector2D pivot;

    // nine-slice
    bool  bNineSlice = false;
    int   nBorder = 0;

    // shimmer: shader on the GPU renderer, CPU lines as the fallback. The normal
    // map is looked up by key every frame, never held by pointer.
    bool            bShimmer = false;
    std::string     m_normalMapKey;
    ShimmerUniforms m_shimmerStyle;

    void drawSprite(SDL_FRect srcRect);   // nine-slice / flip / rotate draw
    void drawShimmerCPU();                // fallback when shaders are unavailable

public:
    SpriteRenderer(const std::string& strTexName, SDL_Color color = { 255, 255, 255, 255 });
    void initialize(); // (when all resources are guaranteed to be loaded).

    ~SpriteRenderer();

    void perform() override;
    void drawWidget() override;

    // setters
    void setTexture(SDL_Texture* pTexture);
    void setPosition(float x, float y);

    //remove later if set scale is what this should do
    void setSize(float w, float h);
    void setFlipX(bool flipX);
    void setFlipY(bool flipY);
    void setAngle(double dAngle);
    void setPivot(Vector2D pivot);
    void setCropRect(SDL_FRect mCropRect);
    void setNineSlice(bool bEnabled, int nBorder);

    // normalMapKey = name the normal map was loaded under in TextureManager
    // (empty = no normal map: plain moving glint).
    void setShimmer(bool bEnabled, const std::string& normalMapKey = "");
    void setShimmerStyle(const ShimmerUniforms& style);   // phase/flip are overwritten per frame

    // getters
    SDL_Texture* getTexture();
    SDL_FRect getRect() const;
    SDL_FRect getCropRect() const;
    bool getflipX();
    bool getFlipY();
    double getAngle();
    Vector2D getPivot();

};