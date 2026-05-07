#include "StoreItem.h"
#include "Sprite.h"
#include "SpriteRenderer.h"
#include "Text.h"
#include "GUIButton.h"
#include "GUIToggle.h"
StoreItem::StoreItem(std::string strName, int nPrice, Vector2D vecPos) :AGameObject(strName)
{
	this->nPrice = nPrice;
	this->vecPos = vecPos;
}

StoreItem::~StoreItem()
{

}

void StoreItem::initialize()
{
	Sprite* pItemBackground = new Sprite("ItemBackground", "UI_Panel", Vector2D(0, 0), Vector2D(1, 1), 0.0f, false);
	this->attachChild(pItemBackground);
	SpriteRenderer* pRenderer = (SpriteRenderer*)pItemBackground->findComponentByName("SpriteRenderer");
	pRenderer->setNineSlice(true, 42);
	pItemBackground->setPos(this->vecPos);
	pItemBackground->setScale(Vector2D(3.5f, 1));
	if(this->getName() == "Coins")
	{
		Sprite* pItemSprite = new Sprite("CoinSprite", "Coin", Vector2D(0, 0), Vector2D(1, 1), 0.0f, false);
		pItemBackground->attachChild(pItemSprite);
		pItemSprite->setLocalPos(Vector2D(-200, 0));
		pItemSprite->setScale(Vector2D(.75, .75));

		Text* pNameText = new Text("CoinText", "CurseCasual.ttf", 35);
		pNameText->setMessage("Coinsx100");
		pItemBackground->attachChild(pNameText);
		pNameText->setLocalPos(Vector2D(-75, 0));
		pNameText->setScale(Vector2D(1, 1));

		Sprite* pCoinSprite = new Sprite("CoinSprite", "Coin", Vector2D(0, 0), Vector2D(1, 1), 0.0f, false);
		pItemBackground->attachChild(pCoinSprite);
		pCoinSprite->setLocalPos(Vector2D(35, 0));
		pCoinSprite->setScale(Vector2D(.5, .5));

		Text* pPriceText = new Text("PriceText", "CurseCasual.ttf", 25);
		pPriceText->setMessage("100");
		pItemBackground->attachChild(pPriceText);
		pPriceText->setLocalPos(Vector2D(75, 0));
		pPriceText->setScale(Vector2D(1, 1));
	}
	GUIButton* pBuyButton = new GUIButton(this->getName(), "Blue_Button");
	GUIToggle* pBuyToggle = new GUIToggle(EventKey::SHOP_PURCHASE);
	pBuyButton->attachComponent(pBuyToggle);
	pItemBackground->attachChild(pBuyButton);
	pBuyButton->setLocalPos(Vector2D(180, 0));
	pBuyButton->setScale(Vector2D(.5, .5));

	Text* pBuyText = new Text("CoinText", "CurseCasual.ttf", 35);
	pBuyText->setMessage("Buy");
	pBuyButton->attachChild(pBuyText);
	pBuyText->setLocalPos(Vector2D(0, 0));
	pBuyText->setScale(Vector2D(1, 1));
	
}
