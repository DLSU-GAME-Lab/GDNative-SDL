#pragma once
#include "AGameObject.h"
#include "Text.h"
#include "SDL3/SDL.h"
#include "cmath"
#include "SpriteRenderer.h"
class ScrollView :public AGameObject
{
private:
	Text* pText;
	int nScrollOffset;
	int nContentHeight;
	SDL_Rect rectViewport;
	std::string strMessage;
public:
	ScrollView(std::string strName, std::string strMessage);
	~ScrollView();

	void initialize() override;
	void draw(SDL_Renderer* pRenderer) override;
	SDL_FRect getGlobalBounds() override;

public:
	void setScrollOffset(int nOffset);
	int getScrollOffset() const;
	int getContentHeight()const;
	void setContentHeight(int nContentHeight);
	int getViewHeight() const;
	
};

