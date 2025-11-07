#include "ScrollView.h"

ScrollView::ScrollView(std::string strName, std::string strMessage):AGameObject(strName)
{
	this->nScrollOffset = 0;
	this->nContentHeight = 0;
	this->rectViewport = SDL_Rect();
	this->pText = NULL;
	this->strMessage = strMessage;
}

ScrollView::~ScrollView()
{
}

void ScrollView::initialize()
{
	if (this->getParent()) {
		SpriteRenderer* pRenderer = (SpriteRenderer*)this->getParent()->findComponentByName("SpriteRenderer");
		SDL_FRect rectBounds = pRenderer->getRect();
		this->fWindowSize = Vector2D(rectBounds.x,rectBounds.y);
		this->rectViewport = { 0, 0, static_cast<int>(fWindowSize.x), static_cast<int>(fWindowSize.y) };
	}

	this->pText = new Text("ScrollableText", "JainiPurva-Regular.ttf", 45, 0, false);
	this->pText->setPos(Vector2D(0, 0));
	this->pText->setScale(Vector2D(1, 1));
	this->pText->setMessage(this->strMessage);
	this->attachChild(this->pText);
	this->nContentHeight = ((DialogueRenderer*)this->pText->findComponentByName("DialogueRenderer"))->getTextHeight();
}

void ScrollView::draw(SDL_Renderer* pRenderer)
{
	SDL_SetRenderClipRect(pRenderer, &rectViewport);

	for (AGameObject* child : vecChildren) {
		Vector2D originalPos = child->getPos();
		Vector2D scrolledPos = Vector2D(originalPos.x, originalPos.y - nScrollOffset);
		child->setPos(scrolledPos);
		child->draw(pRenderer);
		child->setPos(originalPos); // Restore position
	}

	SDL_SetRenderClipRect(pRenderer, nullptr);

}

SDL_FRect ScrollView::getGlobalBounds()
{
	return SDL_FRect{ fVecTranslate.x,fVecTranslate.y, fWindowSize.x,fWindowSize.y };
}

void ScrollView::setScrollOffset(int nOffset)
{
	nScrollOffset = std::max(0, std::min(nOffset, nContentHeight - rectViewport.h));

}

int ScrollView::getScrollOffset() const
{
	return this->nScrollOffset;
}

int ScrollView::getContentHeight() const
{
	return this->nContentHeight;
}

void ScrollView::setContentHeight(int nContentHeight)
{
	this->nContentHeight = nContentHeight;
}

int ScrollView::getViewHeight() const
{
	return this->nContentHeight;
}
