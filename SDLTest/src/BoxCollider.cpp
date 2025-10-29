#include "BoxCollider.h"
#include "AGameObject.h"


BoxCollider::BoxCollider(std::string strName) : ACollider(strName)
{
	this->size = SDL_FRect(0.f, 0.f, 0.f, 0.f);

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

	if(bCollisionX && bCollisionY)
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
				this->intersection.x = fRightB - fLeftA;
			}
			else
			{
				this->bCollideRight = true;
				this->intersection.x = fRightA - fLeftB;
			}
		}
		else
		{
			if (deltaY > 0)
			{
				this->bCollideBottom = true;
				this->intersection.y = fTopA - fBotB;
			}
			else
			{
				this->bCollideTop = true;
				this->intersection.y = fTopB - fBotA;
			}
		}
	}

	return bCollisionX && bCollisionY;
}


SDL_FRect BoxCollider::getSize() const
{
	return this->size;
}

void BoxCollider::setSize(SDL_FRect size)
{
	this->size = size;
}
SDL_FRect BoxCollider::getGlobalBounds()
{
	Vector2D pos = this->pOwner->getPos();
	Vector2D scale = this->pOwner->getScale();

	SDL_FRect bounds = {};
	bounds.w = this->size.w * scale.x;
	bounds.h = this->size.h * scale.y;
	bounds.x = pos.x - (bounds.w / 2.0f);
	bounds.y = pos.y - (bounds.h / 2.0f);

	return bounds;
}