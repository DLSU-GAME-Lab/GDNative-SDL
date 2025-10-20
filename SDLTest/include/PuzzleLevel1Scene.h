#pragma once
#include "AScene.h"
class PuzzleLevel1Scene : public AScene
{
public:
    PuzzleLevel1Scene();
    ~PuzzleLevel1Scene();

public:
    virtual void onLoadResources() override;
    virtual void onLoadObjects() override;
    virtual void onUnloadResources() override;
};

