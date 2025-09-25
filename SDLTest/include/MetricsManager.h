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

    // CPU tracking
    FILETIME prevSysKernel{}, prevSysUser{};
    FILETIME prevProcKernel{}, prevProcUser{};
    bool cpuInitialized = false;
    double cpuUsage = 0.0;

    // Memory tracking
    SIZE_T memoryUsage = 0;

    // Toggles
    bool showFPS = true;
    bool showCPU = true;
    bool showMemory = true;

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

    float getFPS() const { return fps; }
    double getCPUUsage() const { return cpuUsage; }
    SIZE_T getMemoryUsage() const { return memoryUsage; }

    // Singleton
    static void initialize();
    static void destroy();
    static MetricsManager* getInstance();

private:
    MetricsManager() {}
    ~MetricsManager() {}
};
