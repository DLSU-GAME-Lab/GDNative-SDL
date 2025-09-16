#include "InspectorScreen.h"
#include "SceneManager.h"
#include "GameObjectManager.h"
#include <cmath>

void InspectorScreen::setSelectedObject(AGameObject* selectedObject)
{
    this->selectedObject = selectedObject;
}

AGameObject* InspectorScreen::getSelectedObject()
{
    return this->selectedObject;
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

    if (ImGui::BeginChild("Transform", ImVec2(0, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AutoResizeY))
    {
        this->showTransform();
        ImGui::EndChild();
    }

	ImGui::End();
}

void InspectorScreen::showTransform()
{
    if (this->selectedObject == NULL)
    {
        ImGui::Text("No game object selected.");
    }
    else
    {
        ImGui::Text(this->selectedObject->getName().c_str());
        Vector2D pos = this->selectedObject->getPos();
        float rot = this->selectedObject->getRot();
        Vector2D scale = this->selectedObject->getScale();

        float scenePos[] = { pos.x, pos.y };
        float sceneRot = rot;
        float sceneScale[] = { scale.x, scale.y };

        if (ImGui::InputFloat2("Position", scenePos) &&
            ImGui::IsItemDeactivatedAfterEdit())
        {
            this->selectedObject->setPos(Vector2D(scenePos[0], scenePos[1]));
        }

        if (ImGui::InputFloat("Rotation", &sceneRot) &&
            ImGui::IsItemDeactivatedAfterEdit())
        {
            this->selectedObject->setRot(sceneRot);
        }

        if (ImGui::InputFloat2("Scale", sceneScale) &&
            ImGui::IsItemDeactivatedAfterEdit())
        {
            this->selectedObject->setScale(Vector2D(sceneScale[0], sceneScale[1]));
        }
    }
}
