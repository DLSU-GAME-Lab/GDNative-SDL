#pragma once
#include "AComponent.h"
class Gravity:public AComponent
{
private:
	float fGravValue;
	bool bGrounded;
public:
	Gravity(float fGravValue);
	~Gravity();
public:
	void perform() override;
public:
	void setGravValue(float fGravValue);
	float getGravValue();
	void setGrounded(bool bGrounded);
	bool isGrounded();
};

