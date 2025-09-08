#pragma once

#include "AScene.h"
#include "EnumSceneTag.h"

class TitleScene : public AScene
{
public:
    TitleScene();
    ~TitleScene();

    virtual void onLoadResources() override;
    virtual void onLoadObjects() override;
    virtual void onUnloadResources() override;
    virtual void update(float deltaTime) override;
};
