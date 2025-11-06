#include "BoxCollider.h"
#include "AGameObject.h"


BoxCollider::BoxCollider(std::string strName) : ACollider(strName)
{
	this->bCollideLeft = false;
	this->bCollideRight = false;
	this->bCollideBottom = false;
	this->bCollideTop = false;
}
void BoxCollider::onAttach()
{
	
}
void BoxCollider::perform()
{
	
}

bool BoxCollider::isColliding(ACollider* pCollider)
{
	SDL_FRect CBoundsA = this->getGlobalBounds();
	SDL_FRect CBoundsB = pCollider->getGlobalBounds();

	float fLeftA, fLeftB;
	float fRightA, fRightB;
	float fTopA, fTopB;
	float fBotA, fBotB;

	//RectA 
	fLeftA = CBoundsA.x;
	fRightA = CBoundsA.x + CBoundsA.w;
	fBotA = CBoundsA.y;
	fTopA = CBoundsA.y + CBoundsA.h;

	//RectB
	fLeftB = CBoundsB.x;
	fRightB = CBoundsB.x + CBoundsB.w;
	fBotB = CBoundsB.y;
	fTopB = CBoundsB.y + CBoundsB.h;

	bool bIntersectX = fLeftA <= fRightB && fRightA >= fLeftB;
	bool bIntersectY = fBotA <= fTopB && fTopA >= fBotB;

	if (bIntersectX && bIntersectY && !pCollider->getIsTrigger())
	{
		float fBot = fTopB - fBotA;
		float fTop = fTopA - fBotB;
		float fLeft = fRightB - fLeftA;
		float fRight = fRightA - fLeftB;

		if (std::min(fBot, fTop) < std::min(fLeft, fRight))
		{
			if (fBot < fTop)
			{
				this->bCollideBottom = true;
				this->intersection.y = fTopB - fBotA;
			}
			else if (fTop < fBot)
			{
				this->bCollideTop = true;
				this->intersection.y = fBotB - fTopA;
			}
		}
		else
		{
			if (fLeft < fRight)
			{
				this->bCollideLeft = true;
				this->intersection.x = fRightB - fLeftA;
			}
			else if (fRight < fLeft)
			{
				this->bCollideRight = true;
				this->intersection.x = fLeftB - fRightA;
			}
		}
	}

	return bIntersectX && bIntersectY;
}

void BoxCollider::onCollisionEnter(ACollider* pCollider)
{
	ACollider::onCollisionEnter(pCollider);
}

void BoxCollider::onCollisionContinue(ACollider * pCollider)
{
	ACollider::onCollisionContinue(pCollider);
}

void BoxCollider::onCollisionExit(ACollider * pCollider)
{
	ACollider::onCollisionExit(pCollider);
	this->bCollideLeft = false;
	this->bCollideRight = false;
	this->bCollideBottom = false;
	this->bCollideTop = false;
}


Vector2D BoxCollider::getSize() const
{
	return this->size;
}

void BoxCollider::setSize(Vector2D size)
{
	this->size = size;
}
Vector2D BoxCollider::getOffset() const
{
	return this->offset;
}

void BoxCollider::setOffset(Vector2D offset)
{
	this->offset = offset;
}

SDL_FRect BoxCollider::getGlobalBounds()
{
	Vector2D pos = this->pOwner->getPos();
	Vector2D scale = this->pOwner->getScale();

	SDL_FRect bounds = {};
	bounds.w = this->size.x * scale.x;
	bounds.h = this->size.y * scale.y;
	bounds.x = this->offset.x + pos.x - (bounds.w * 0.5f);
	bounds.y = this->offset.x + pos.y - (bounds.h * 0.5f);

	return bounds;
}

bool BoxCollider::isCollidedLeft() const
{
	return this->bCollideLeft;
}

bool BoxCollider::isCollidedRight() const
{
	return this->bCollideRight;
}

bool BoxCollider::isCollidedBottom() const
{
	return this->bCollideBottom;
}

bool BoxCollider::isCollidedTop() const
{
	return this->bCollideTop;
}
