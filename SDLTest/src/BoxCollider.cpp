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
	fBotA = CBoundsA.y;
	fTopA = CBoundsA.y + CBoundsA.h;

	//RectB
	fLeftB = CBoundsB.x;
	fRightB = CBoundsB.x + CBoundsB.w;
	fBotB = CBoundsB.y;
	fTopB = CBoundsB.y + CBoundsB.h;

	bool bIntersectX = fLeftA < fRightB && fRightA > fLeftB;
	bool bIntersectY = fBotA < fTopB && fTopA > fBotB;

	if (bIntersectX && bIntersectY)
	{
		Vector2D posA = this->pOwner->getPos();
		Vector2D posB = pCollider->getOwner()->getPos();
		Vector2D subtractPos = posA - posB;

		if (std::fabs(subtractPos.x) < std::fabs(subtractPos.y))
		{
			if (subtractPos.x > 0.0f)
			{
				this->bCollideLeft = true;
				this->intersection.x = fRightB - fLeftA;
			}
			else if (subtractPos.x < 0.0f)
			{
				this->bCollideRight = true;
				this->intersection.x = fLeftB - fRightA;
			}
		}
		else
		{
			if (subtractPos.y > 0.0f)
			{
				this->bCollideBottom = true;
				this->intersection.y = fTopB - fBotA;
			}
			else if (subtractPos.y < 0.0f)
			{
				this->bCollideTop = true;
				this->intersection.y = fBotB - fTopA;
			}
		}

		if (this->pOwner->getName() == "Player")
		{
			std::cout << "Self: " << fLeftA << " " << fRightA << " " << fBotA << " " << fTopA << std::endl;
			std::cout << "Other: " << fLeftB << " " << fRightB << " " << fBotB << " " << fTopB << std::endl;
			std::cout << "intersection: " << this->intersection << std::endl;
		}
	}

	return bIntersectX && bIntersectY;
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
	bounds.x = pos.x - (bounds.w * 0.5f);
	bounds.y = pos.y - (bounds.h * 0.5f);

	return bounds;
}