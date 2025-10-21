#pragma once
#include "AGameObject.h"
#include "Collider.h"
#include "ACollisionListener.h"
class Player : public AGameObject, ACollisionListener
{
public:
	Player(Vector2D fVecTranslate, Vector2D fVecScale, float fRot);

	void initialize() override;

	 //Inherited via ACollisionListener
	void onCollisionEnter(Collider* pCollider) override;

	void onCollisionContinue(Collider* pCollider) override;

	void onCollisionExit(Collider* pCollider) override;

};

