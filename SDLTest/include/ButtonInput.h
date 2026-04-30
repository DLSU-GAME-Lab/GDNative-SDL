#pragma once
#include "AGeneralInput.h"
#include "SpriteRenderer.h"

class ButtonInput : public AGeneralInput
{
private:
	SpriteRenderer* pSprite;
	Vector2D mousePos;

	bool bHolding;
	bool bDragging;
	bool bClicked;
	bool bRightClick;
	bool bLeftClick;
    float logicalX;
    float logicalY;

public:
	ButtonInput(SpriteRenderer* pSprite);
	~ButtonInput();

	void perform() override;

	Vector2D getMousePos() const;
	Vector2D getMouseWorldPos() const;
	bool getHolding() const;
	bool getDragging() const;
	bool getClicked() const;
	void setClicked(bool bClicked);
	bool isRightClick();
	void setRightClick(bool bRightClick);
	bool isLeftClick();
	void setLeftClick(bool bLeftClick);
    bool contains() const;
    bool contains(const Vector2D& pos) const;


private:
	virtual void onMouseHovered(Vector2D mousePos);
	virtual void onMouseButtonDown(Uint8 mouseButton);
	virtual void onMouseButtonUp(Uint8 mouseButton);

	//bool contains(const Vector2D& pos) const;
};

