#pragma once
#include "AGameObject.h"
#include "Prop.h"
class StoryWindow: public AGameObject 
{
public:
	StoryWindow(std::string strName);
	~StoryWindow();

	// Inherited via AGameObject
	void initialize() override;
};

