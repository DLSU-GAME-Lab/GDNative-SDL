#pragma once
#include "imgui.h"

class CustomStyles
{
public:
	void StyleColorsCrimson();
	void StyleColorsEvangelion();
	void StyleColorsDarkForest();

private:
	ImVec4 Alpha(const ImVec4& color, float a);
};
