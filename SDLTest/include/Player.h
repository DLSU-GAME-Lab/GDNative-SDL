#pragma once
#include "AGameObject.h"
#include "BoxCollider.h"
#include "ICollisionListener.h"
class Player : public AGameObject, ICollisionListener
{
public:
	Player(Vector2D fVecTranslate, Vector2D fVecScale, float fRot);

	void initialize() override;

	 //Inherited via ACollisionListener
	void onCollisionEnter(ACollider* pCollider) override;

	void onCollisionContinue(ACollider* pCollider) override;

	void onCollisionExit(ACollider* pCollider) override;

};

