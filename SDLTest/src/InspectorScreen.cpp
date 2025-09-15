#include "InspectorScreen.h"
#include "GameObjectManager.h"

InspectorScreen::InspectorScreen() : AUIScreen("INSPECTOR_SCREEN")
{
	this->selectedObject = NULL;
}

InspectorScreen::~InspectorScreen()
{

}

void InspectorScreen::DrawUI()
{
	ImGui::Begin("Inspector", NULL, ImGuiWindowFlags_NoCollapse);
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
        bool recalculate = false;

        float scenePos[2] = { pos.x, pos.y };
        float sceneRot = { rot };
        float sceneScale[2] = { scale.x, scale.y };

        if (ImGui::InputFloat2("Position", scenePos) &&
            ImGui::IsItemDeactivatedAfterEdit())
        {
            recalculate = true;
        }

        if (ImGui::InputFloat("Rotation", &sceneRot) &&
            ImGui::IsItemDeactivatedAfterEdit())
        {
            recalculate = true;
        }

        if (ImGui::InputFloat2("Scale", sceneScale) &&
            ImGui::IsItemDeactivatedAfterEdit())
        {
            recalculate = true;
        }

        if (recalculate)
        {
            this->selectedObject->setPos(Vector2D(scenePos[0], scenePos[1]));
            this->selectedObject->setRot(sceneRot);
            this->selectedObject->setScale(Vector2D(sceneScale[0], sceneScale[1]));
        }
    }
	ImGui::End();
}
