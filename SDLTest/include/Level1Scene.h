#pragma once
#include "AScene.h"
class Level1Scene : public AScene
{
public:
    Level1Scene();
    ~Level1Scene();

public:
    virtual void onLoadResources() override;
    virtual void onLoadObjects() override;
    virtual void onUnloadResources() override;
};

