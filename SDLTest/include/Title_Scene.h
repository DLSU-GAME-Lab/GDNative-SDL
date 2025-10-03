#pragma once

#include "AScene.h"
#include "EnumSceneTag.h"
#include <SDL3/SDL.h>

class Title_Scene : public AScene {
public:
    Title_Scene();
    ~Title_Scene() override = default;
public:
    void onLoadResources() override;
    void onLoadObjects() override;
    void onUnloadResources() override;
    void onUnloadObjects() override;
public:
    void loadText();
    void loadAnimatedTextures();
    void loadSceneTextures();
    void createButtons();
    void createScene();

};
