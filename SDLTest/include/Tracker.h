#pragma once
#include "string"
#include "Prop.h"
#include "AGameObject.h"
#include "Text.h"
class Tracker:public AGameObject
{
private:
	std::string strName;
	std::string strImageName;
	int nTargetDestroy;
	int nTargetLeft;
public:
	Tracker(std::string strName, std::string strImageName, int nTargetDestroy);
	~Tracker();
public:
	void initialize();
public:
	std::string getName();
	int getTargetLeft();
	void setTargetLeft(int nTargetLeft);
};

