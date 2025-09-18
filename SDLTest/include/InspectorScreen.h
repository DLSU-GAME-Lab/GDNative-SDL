#pragma once
#include "AUIScreen.h"
#include "AGameObject.h"

namespace Editor
{
	class InspectorScreen : public AUIScreen
	{
	private:
		AGameObject* selectedObject;
		Vector2D mousePos;

	public:
		void setSelectedObject(AGameObject* selectedObject);
		AGameObject* getSelectedObject();
		void setMousePos(Vector2D mousePos);

	protected:
		InspectorScreen();
		~InspectorScreen();
		void DrawUI() override;

		friend class UIManager;

	private:
		void showCamera(ImGuiChildFlags childFlags);
		void showTransform(ImGuiChildFlags childFlags);
	};

}