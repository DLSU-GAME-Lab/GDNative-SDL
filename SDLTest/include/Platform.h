#pragma once
#include "AGameObject.h"
#include "PhysicsSystem.h"
#include "ICollisionListener.h"
#include "BoxCollider.h"
class Platform:public AGameObject, ICollisionListener
{
private:
	SDL_FRect bounds;
	RectangleShape* Rect;
public:
	Platform(const std::string& strName, Vector2D fVecTranslate, Vector2D fVecSize, float fRot);
	~Platform() override;

	void initialize() override;


	// Inherited via ACollisionListener
	void onCollisionEnter(ACollider* pCollider) override;
	void onCollisionContinue(ACollider* pCollider) override;
	void onCollisionExit(ACollider* pCollider) override;

};

