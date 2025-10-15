#pragma once
#include "string"
#include "Prop.h"
#include "AGameObject.h"
#include "Text.h"
#include "EnumGemType.h"
class Tracker:public AGameObject
{
private:
	std::string strImageName;
	int nTargetDestroy;
	int nTargetLeft;
	GemType EGem;

public:
	Tracker(std::string strName, std::string strImageName, GemType EGem, int nTargetDestroy);
	~Tracker();
public:
	void initialize();
	void updateScore(int nSubtractValue);

public:
	std::string getName();
	GemType getGemType();
	
};

