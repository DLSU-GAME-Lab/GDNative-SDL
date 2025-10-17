#pragma once
#include "AGameObject.h"
#include "PhysicsManager.h"
#include "ACollisionListener.h"

class Platform:public AGameObject, ACollisionListener
{
private:
	std::string strImageName;
	SDL_FRect bounds;
	RectangleShape* Rect;
public:
	Platform(const std::string& strName, Vector2D fVecTranslate, Vector2D fVecSize, float fRot);
	~Platform();

	void initialize() override;

	SDL_FRect getGlobalBounds() override;

	// Inherited via ACollisionListener
	void onCollisionEnter(Collider* pCollider) override;
	void onCollisionContinue(Collider* pCollider) override;
	void onCollisionExit(Collider* pCollider) override;

};

