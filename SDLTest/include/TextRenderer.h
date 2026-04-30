#pragma once
#include "SDL3_ttf/SDL_ttf.h"
#include "ARenderer.h"
#include "FontManager.h"
#include "RendererContext.h"
#include "AGameObject.h"
#include "SpriteRenderer.h"
class TextRenderer:public ARenderer
{
private:
    std::string strFontType;
    int nFontSize;
    std::string strText;

	SDL_Texture* pTexture;
    SDL_FRect mDestRect;

    bool flipX, flipY;
    double dAngle;
    Vector2D texSize;
    Vector2D pivot;
    bool bCropEnabled;
    float fScrollOffset;
    float fViewHeight;
public:
	TextRenderer();
	~TextRenderer();

	void initialize(); // (when all resources are guaranteed to be loaded).
	void perform() override;
    void drawWidget() override;
    void loadFromText(std::string fontType, int nFontSize, std::string strText, SDL_Color textColor);
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
    void setColor(SDL_Color color) override;

};

