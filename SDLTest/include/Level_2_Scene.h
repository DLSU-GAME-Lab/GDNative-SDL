#pragma once
#include "AScene.h"
#include "EnumSceneTag.h"
#include <SDL3/SDL.h>
#include "Background.h"
#include "UIButton.h"

class Level_2_Scene : public AScene
{
public:
    Level_2_Scene();
    ~Level_2_Scene() override = default;

    void onLoadResources() override;
    void onLoadObjects() override;
    void onUnloadResources() override;
    void onUnloadObjects() override;

    void update(float deltaTime) override;
    void render(SDL_Renderer* pRenderer) override;
};
