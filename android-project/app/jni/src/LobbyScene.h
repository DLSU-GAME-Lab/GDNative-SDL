#pragma once
#include "AScene.h"
#include "EnumSceneTag.h"

class LobbyScene : public AScene
{
public:
    LobbyScene();
    ~LobbyScene();

public:
    virtual void onLoadResources() override;
    virtual void onLoadObjects() override;
    virtual void onUnloadResources() override;

};

