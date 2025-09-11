#pragma once

#include "AButton.h"
#include "EnumSceneTag.h"

class UIButton : public AButton {
public:
    UIButton(std::string objName, std::string textureKey,
        Vector2D fVecTranslate, Vector2D fVecScale,
        float rotation, bool isUI, SceneTag eTargetScene);

    ~UIButton() override = default;
    void initialize() override;

    void OnPressed(SDL_MouseButtonEvent eMouseEvent) override;
    void OnReleased(SDL_MouseButtonEvent eMouseEvent) override;
    void OnHovered() override;

private:
    SceneTag m_eTargetScene;
    std::string m_textureKey;
};
