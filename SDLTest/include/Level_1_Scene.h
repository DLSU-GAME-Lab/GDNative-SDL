#pragma once
#include "AScene.h"
#include "EnumSceneTag.h"
#include <SDL3/SDL.h>
#include "Background.h"

class Level_1_Scene : public AScene
{
public:
    Level_1_Scene();
    ~Level_1_Scene() override = default;
    void onLoadResources() override;
    void onLoadObjects() override;
    void onUnloadResources() override;
    void onUnloadObjects() override;
public:
    void loadEmptyObjects();
    void loadGUI();

};

