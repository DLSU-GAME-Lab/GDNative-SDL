#pragma once
#include <windows.h>
#include <psapi.h>
#include <string>
#include <iostream>
#include <fstream>

class MetricsManager {
private:
    static MetricsManager* P_SHARED_INSTANCE;

    // FPS tracking
    unsigned long frameCount = 0;
    float fps = 0.0f;
    LARGE_INTEGER qpcLastTime{};
    double qpcFrequency = 0.0;

    // FPS stats
    float avgFPS = 0.0f;
    float minFPS = FLT_MAX;
    float maxFPS = 0.0f;

    // CPU tracking
    FILETIME prevSysKernel{}, prevSysUser{};
    FILETIME prevProcKernel{}, prevProcUser{};
    bool cpuInitialized = false;
    double cpuUsage = 0.0;

    // Memory tracking
    SIZE_T memoryUsage = 0;

    // GPU tracking (external only, set 0 here)
    double gpuUsage = 0.0;

    // Load Time tracking
    double loadTimeSec = 0.0;
    LARGE_INTEGER loadStart{};

    // INPUT Lag tracking
    double inputLagMs = 0.0;
    LONGLONG inputTimestamp = 0;

    // Toggles
    bool showFPS = true;
    bool showCPU = true;
    bool showMemory = true;
    bool showGPU = true; 
    bool showInputLag = true; 
    bool showLoadTime = true;

    // History buffers
    static const int HISTORY_SIZE = 100;
    float fpsHistory[HISTORY_SIZE] = {};
    float cpuHistory[HISTORY_SIZE] = {};
    int offset = 0;

public:
    void update();
    void logMetrics();
    void drawGUI();
    void exportCSV(const std::string& filename = "metrics_export.csv");

    // getters
    float getFPS() const { return fps; }
    double getCPUUsage() const { return cpuUsage; }
    SIZE_T getMemoryUsage() const { return memoryUsage; }
    double getGPUUsage() const { return gpuUsage; } // always 0 in code, real gpu logged externally
    double getInputLag() const { return inputLagMs; }
    double getLoadTime() const { return loadTimeSec; }

    // helpers for load time
    void startLoadTimer();
    void endLoadTimer();

    // helpers for input lag
    void recordInputEvent(); // call on key press or mouse click
    void markInputHandled(); // call when the frame reacts to input

    // Singleton
    static void initialize();
    static void destroy();
    static MetricsManager* getInstance();

private:
    MetricsManager() {}
    ~MetricsManager() {}
};