#pragma once
#include "ARenderer.h"
class ColliderRenderer: public ARenderer
{
private:

public:
	ColliderRenderer(std::string strName);
	// Inherited via ARenderer
	void perform() override;
};

