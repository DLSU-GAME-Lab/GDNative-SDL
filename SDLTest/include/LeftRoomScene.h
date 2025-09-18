#pragma once
#pragma once
#include "AScene.h"
#include "EnumSceneTag.h"
class LeftRoomScene :public AScene
{
public: 
	LeftRoomScene();
	~LeftRoomScene();
public:
	virtual void onLoadResources() override;
	virtual void onLoadObjects() override;
	virtual void onUnloadResources() override;
};

