#pragma once
#include "AComponent.h"
#include "ButtonInput.h"
#include "Text.h"
#include "DialogueRenderer.h"
class Scroller:public AComponent
{
	private:
		Text* pScrollableText;
		bool bDragging;
		float fPrevMouseY;
		float fPrevMouseX;
	public:
		Scroller(std::string strName, Text* pScrollableText);
		~Scroller();
	public:

		// Inherited via AComponent
		void perform() override;
};

