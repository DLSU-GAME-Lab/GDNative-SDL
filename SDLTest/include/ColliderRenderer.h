#pragma once
#include "ARenderer.h"
#include "ACollider.h"
#include "AGameObject.h"

class ColliderRenderer: public ARenderer
{
private:
	ACollider* pCollider;

public:
	ColliderRenderer(ACollider* pCollider);
	// Inherited via ARenderer
	void perform() override;
	void drawWidget() override;
};

