#pragma once
#include "AScene.h"
#include "EnumSceneTag.h"
#include <SDL3/SDL.h>
#include "Background.h"

class Level_3_Scene : public AScene
{
public:
    Level_3_Scene();
    ~Level_3_Scene() override = default;

    void onLoadResources() override;
    void onLoadObjects() override;
    void onUnloadResources() override;
    void onUnloadObjects() override;
public:
    void loadEmptyObjects();
    void loadGUI();
};
