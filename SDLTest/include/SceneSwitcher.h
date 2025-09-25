#pragma once
#include "AComponent.h"
#include "EnumSceneTag.h"
#include "ButtonInput.h"

class SceneSwitcher : public AComponent
{
private:
	SceneTag ETag;
	ButtonInput* pInput;

public:
	SceneSwitcher(SceneTag ETag);
	~SceneSwitcher();

	void perform() override;
};

