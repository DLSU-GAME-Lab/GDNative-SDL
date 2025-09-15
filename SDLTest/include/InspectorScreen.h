#pragma once
#include "AUIScreen.h"
#include "AGameObject.h"

class InspectorScreen : public AUIScreen
{
protected:
	InspectorScreen();
	~InspectorScreen();
	void DrawUI() override;

	friend class UIManager;

private:
	AGameObject* selectedObject;

	void showTransform();
};

