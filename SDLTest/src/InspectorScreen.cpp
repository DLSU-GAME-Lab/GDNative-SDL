#include "InspectorScreen.h"
#include "SceneManager.h"
#include "GameObjectManager.h"
#include "CameraManager.h"
#include <cmath>

void InspectorScreen::setSelectedObject(AGameObject* selectedObject)
{
    this->selectedObject = selectedObject;
}

AGameObject* InspectorScreen::getSelectedObject()
{
    return this->selectedObject;
}

void InspectorScreen::setMousePos(Vector2D mousePos)
{
    this->mousePos = mousePos;
}

float InspectorScreen::getCamMoveSpeed() const
{
    return this->fCamMoveSpeed;
}

float InspectorScreen::getCamScaleSpeed() const
{
    return this->fCamScaleSpeed;
}

InspectorScreen::InspectorScreen() : AUIScreen("INSPECTOR_SCREEN")
{
	this->selectedObject = NULL;
}

InspectorScreen::~InspectorScreen()
{

}

void InspectorScreen::DrawUI()
{
	ImGui::Begin("Inspector", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);
    const int scenes = SceneManager::getInstance()->getRegisteredSceneAmount();
    int sceneIndex = (int)SceneManager::getInstance()->getLoadedSceneTag();

    std::string scenesText = "Scenes registered: " + std::to_string(scenes);
    ImGui::Text(scenesText.c_str());
    if (ImGui::InputInt("Scene Index", &sceneIndex))
    {
        selectedObject = NULL;
        sceneIndex = SDL_clamp(sceneIndex, 0, scenes - 1);
        SceneManager::getInstance()->loadScene((SceneTag)sceneIndex);
    }

    Camera* cam = CameraManager::getInstance()->getCurrentCamera();

    Vector2D mouseWorldPos;
    mouseWorldPos.x = (this->mousePos.x + cam->getPos().x - cam->getHalfWidth()) * cam->getScale().x;
    mouseWorldPos.y = (-(this->mousePos.y - cam->getPos().y - cam->getHalfHeight())) * cam->getScale().y;

    std::string mousePosText = "Mouse Screen Position: (" + std::to_string((int)mousePos.x) + ", " + std::to_string((int)mousePos.y) + ")";
    std::string mouseWorldPosText = "Mouse World Position: (" + std::to_string((int)mouseWorldPos.x) + ", " + std::to_string((int)mouseWorldPos.y) + ")";
    ImGui::Text(mousePosText.c_str());
    ImGui::Text(mouseWorldPosText.c_str());

    ImGuiChildFlags childFlags =
        ImGuiChildFlags_Borders |
        ImGuiChildFlags_AutoResizeY;

    ImGui::Spacing();
    showCamera(childFlags);
    ImGui::Spacing();
    showTransform(childFlags);

    ImGui::End();
}

void InspectorScreen::showCamera(ImGuiChildFlags childFlags)
{
    ImGui::BeginChild("Camera", ImVec2(320, 0), childFlags);
    ImGui::Text("Camera");

    Camera* cam = CameraManager::getInstance()->getCurrentCamera();
    Vector2D pos = cam->getPos();
    Vector2D scale = cam->getScale();
    float rot = cam->getRot();

    float scenePos[] = { pos.x, pos.y };
    float sceneScale[] = { scale.x, scale.y };
    float sceneRot = rot;

    ImGui::DragFloat("Move Speed", &this->fCamMoveSpeed);
    ImGui::DragFloat("Scale Speed", &this->fCamScaleSpeed);
    
    if (ImGui::DragFloat2("Position", scenePos))
    {
        cam->setPos(Vector2D(scenePos[0], scenePos[1]));
    }

    if (ImGui::DragFloat2("Scale", sceneScale, 1.0f, 0.001f, 0.0f))
    {
        cam->setScale(Vector2D(sceneScale[0], sceneScale[1]));
    }

    if (ImGui::DragFloat("Rotation", &sceneRot))
    {
        cam->setRot(sceneRot);
    }

    ImGui::EndChild();
}

void InspectorScreen::showTransform(ImGuiChildFlags childFlags)
{
    ImGui::BeginChild("Transform", ImVec2(0, 0), childFlags);
    if (this->selectedObject == NULL)
    {
        ImGui::Text("No game object selected.");
    }
    else
    {
        std::string objectText;
        if (this->selectedObject->getIsScreenObject())
            objectText = "Screen Object: " + this->selectedObject->getName();
        else
            objectText = "Game Object: " + this->selectedObject->getName();

        ImGui::Text(objectText.c_str());
        Vector2D pos = this->selectedObject->getPos();
        float rot = this->selectedObject->getRot();
        Vector2D scale = this->selectedObject->getScale();

        float scenePos[] = { pos.x, pos.y };
        float sceneRot = rot;
        float sceneScale[] = { scale.x, scale.y };

        if (ImGui::DragFloat2("Position", scenePos))
        {
            this->selectedObject->setPos(Vector2D(scenePos[0], scenePos[1]));
        }

        if (ImGui::DragFloat2("Scale", sceneScale))
        {
            this->selectedObject->setScale(Vector2D(sceneScale[0], sceneScale[1]));
        }

        if (ImGui::DragFloat("Rotation", &sceneRot))
        {
            this->selectedObject->setRot(sceneRot);
        }
    }
    ImGui::EndChild();
}
