#pragma once
#include "SDL3_ttf/SDL_ttf.h"
#include "ARenderer.h"
#include "FontManager.h"
#include "RendererContext.h"
#include "AGameObject.h"
#include "SpriteRenderer.h"
class DialogueRenderer:public ARenderer
{
private:
	SDL_Texture* pTexture;
    SDL_Color mColor;
    SDL_FRect mDestRect;
    bool flipX, flipY;
    double dAngle;
    Vector2D texSize;
    Vector2D pivot;
    bool bCropEnabled;
    float fScrollOffset;
    float fViewHeight;
public:
	DialogueRenderer();
	~DialogueRenderer();

	void initialize(); // (when all resources are guaranteed to be loaded).
	void perform() override;
    void drawWidget() override;
    void loadFromText(std::string strName, std::string fontType, int nFontSize, std::string textureText, SDL_Color textColor);
    void resetText();
    void onAttach() override;
    float getTextHeight();
    void setCropEnabled (bool bbCropEnabled);
public:
    void setPivot(Vector2D pivot);
    float getScrollOffset();
    void setScrollOffset(float fOffset);
    void setViewHeight(float fViewHeight);
    float getViewHeight();


};

