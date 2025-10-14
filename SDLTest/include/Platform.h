#pragma once
#include "AGameObject.h"
#include "PhysicsManager.h"
#include "ACollisionListener.h"

class Platform:public AGameObject, ACollisionListener
{
private:
	std::string strImageName;
public:
	Platform(const std::string& strName, const std::string& strImageName, Vector2D fVecTranslate, Vector2D fVecScale, float fRot);
	~Platform();

	void initialize() override;

	// Inherited via ACollisionListener
	void onCollisionEnter(Collider* pCollider) override;
	void onCollisionContinue(Collider* pCollider) override;
	void onCollisionExit(Collider* pCollider) override;

};

