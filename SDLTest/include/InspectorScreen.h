#pragma once
#include "AUIScreen.h"
#include "AGameObject.h"

class InspectorScreen : public AUIScreen
{
private:
	AGameObject* selectedObject;
	Vector2D mousePos;
	float fCamMoveSpeed = 30.0f;
	float fCamScaleSpeed = 10.0f;

public:
	void setSelectedObject(AGameObject* selectedObject);
	AGameObject* getSelectedObject();
	void setMousePos(Vector2D mousePos);

	float getCamMoveSpeed() const;
	float getCamScaleSpeed() const;

protected:
	InspectorScreen();
	~InspectorScreen();
	void DrawUI() override;

	friend class UIManager;

private:
	void showCamera(ImGuiChildFlags childFlags);
	void showTransform(ImGuiChildFlags childFlags);
};