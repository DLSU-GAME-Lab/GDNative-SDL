#pragma once
#include "AUIScreen.h"
class PerformanceMetricsScreen : public AUIScreen
{
private:
    // Toggles
    bool showFPS = true;
    bool showCPU = true;
    bool showMemory = true;
    bool showGPU = true;
    bool showInputLag = true;
    bool showLoadTime = true;
    bool showThreads = true; // toggle thread view

protected:
	PerformanceMetricsScreen();
	~PerformanceMetricsScreen();

	void DrawUI() override;
};

