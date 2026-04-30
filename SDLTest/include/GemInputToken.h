#pragma once
#include "AGameObject.h"
class GemInputToken:public AGameObject
{
private:
	bool bDropped;
public:
	GemInputToken(std::string strName, Vector2D position, Vector2D scale);

	void initialize() override;

	bool getDropped() const;
	void setDropped(bool bDropped);
};

