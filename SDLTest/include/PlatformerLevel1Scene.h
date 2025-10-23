#pragma once
#include "AScene.h"
class PlatformerLevel1Scene : public AScene
{
public:
    PlatformerLevel1Scene();
    ~PlatformerLevel1Scene();

public:
    virtual void onLoadResources() override;
    virtual void onLoadObjects() override;
    virtual void onUnloadResources() override;
};

