#pragma once
#include "AUIScreen.h"
#include "AGameObject.h"

class InspectorScreen : public AUIScreen
{
public:
	void setSelectedObject(AGameObject* selectedObject);
	AGameObject* getSelectedObject();

protected:
	InspectorScreen();
	~InspectorScreen();
	void DrawUI() override;

	friend class UIManager;

private:
	AGameObject* selectedObject;

	void showCamera();
	void showTransform();
};

