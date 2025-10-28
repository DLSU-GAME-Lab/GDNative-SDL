#include "BoxCollider.h"


BoxCollider::BoxCollider(std::string strName, SDL_FRect initialBounds):ACollider(strName)
{
	this->COffset = SDL_FRect(0.f, 0.f, 0.f, 0.f);
	this->rectShape = RectangleShape{initialBounds.w,initialBounds.h};

}
void BoxCollider::onAttach()
{
	Vector2D fVecPos = this->pOwner->getPos();
	this->rectShape.setPosition(fVecPos.x, fVecPos.y);
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
	fTopA = CBoundsA.y;
	fBotA = CBoundsA.y + CBoundsA.h;

	//RectB
	fLeftB = CBoundsB.x;
	fRightB = CBoundsB.x + CBoundsB.w;
	fTopB = CBoundsB.y;
	fBotB = CBoundsB.y + CBoundsB.h;

	bool bCollisionX = (fLeftA < fRightB) && (fRightA > fLeftB);
	bool bCollisionY = (fTopA < fBotB) && (fBotA > fTopB);

	if(bCollisionX&& bCollisionY)
	{
		float centerAX = fLeftA + (CBoundsA.w / 2.0f);
		float centerAY = fTopA + (CBoundsA.h / 2.0f);
		float centerBX = fLeftB + (CBoundsB.w / 2.0f);
		float centerBY = fTopB + (CBoundsB.h / 2.0f);

		float deltaX = centerAX - centerBX;
		float deltaY = centerAY - centerBY;
		if (std::abs(deltaX) > std::abs(deltaY))
		{
			if (deltaX > 0)
			{
				this->bCollideLeft = true;
			}
			else
			{
				this->bCollideRight = true;

			}
		}
		else
		{
			if (deltaY > 0)
			{
				this->bCollideTop = true;

			}
			else
			{
				this->bCollideBottom = true;

			}
		}
	}


	return bCollisionX && bCollisionY;
}


SDL_FRect BoxCollider::getOffset()
{
	return this->COffset;
}

void BoxCollider::setOffset(SDL_FRect COffset)
{
	this->COffset = COffset;
}
SDL_FRect BoxCollider::getGlobalBounds()
{
	SDL_FRect bounds = this->rectShape.getGlobalBounds();

	// Compute center
	float centerX = bounds.x + bounds.w / 2.0f;
	float centerY = bounds.y + bounds.h / 2.0f;

	// Apply offset to center
	centerX += this->COffset.x;
	centerY += this->COffset.y;

	// Reconstruct bounds from new center
	SDL_FRect CTransform;
	CTransform.w = bounds.w + this->COffset.w;
	CTransform.h = bounds.h + this->COffset.h;
	CTransform.x = centerX - CTransform.w / 2.0f;
	CTransform.y = centerY - CTransform.h / 2.0f;


	return CTransform;
}