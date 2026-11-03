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

    // shimmer: shader on the GPU renderer only, the sprite is drawn plain when
    // shaders are unavailable. The normal map and the mask are looked up by key
    // every frame, never held by pointer.
    bool            bShimmer = false;
    std::string     m_normalMapKey;
    std::string     m_maskMapKey;
    ShimmerUniforms m_shimmerStyle;

    // sweep timing, driven by the clock: one sweep takes 1 / speed seconds,
    // then the sprite rests for the cooldown
    float           m_fShimmerSpeed = 0.45f;
    float           m_fShimmerCooldown = 5.0f;
    float           m_fShimmerOffset = 0.0f;

    void drawSprite(SDL_FRect srcRect);   // nine-slice / flip / rotate draw
    bool getShimmerPhase(float& fPhase) const;   // false while the sprite is resting

public:
    SpriteRenderer(const std::string& strTexName, SDL_Color color = { 255, 255, 255, 255 });
    void initialize(); // (when all resources are guaranteed to be loaded).

    ~SpriteRenderer();

    void perform() override;
    void drawWidget() override;

    // setters
    void setTexture(SDL_Texture* pTexture);
    void setTextureKey(const std::string& textureKey);   // swaps the texture by TextureManager key and refreshes the size
    void setPosition(float x, float y);

    //remove later if set scale is what this should do
    void setSize(float w, float h);
    void setFlipX(bool flipX);
    void setFlipY(bool flipY);
    void setAngle(double dAngle);
    void setPivot(Vector2D pivot);
    void setCropRect(SDL_FRect mCropRect);
    void setNineSlice(bool bEnabled, int nBorder);

    // normalMapKey / maskKey = names the normal map and the mask were loaded under
    // in TextureManager (empty normal map = plain moving glint, empty mask = the
    // whole sprite can shimmer). The mask uses its red channel.
    void setShimmer(bool bEnabled, const std::string& normalMapKey = "", const std::string& maskKey = "");
    // speed = sweeps per second while active (0.45 = a sweep every 2.2s of motion),
    // cooldown = seconds of rest between sweeps, offset = seconds, to desync sprites
    void setShimmerTiming(float fSpeed, float fCooldown, float fOffset = 0.0f);
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