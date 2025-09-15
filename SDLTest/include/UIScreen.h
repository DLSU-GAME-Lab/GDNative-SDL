#pragma once
#include "string";
#include "imgui.h"
#include "imgui_impl_sdl3.h"

class UIManager;

class UIScreen
{
protected:
	UIScreen(std::string name);
	~UIScreen();

	std::string GetName();
	virtual void DrawUI() = 0;

	std::string name;
	bool enabled = true;

	friend class UIManager;
};

