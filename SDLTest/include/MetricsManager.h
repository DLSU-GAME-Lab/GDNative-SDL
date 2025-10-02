#pragma once
#include <string>
#include <iostream>
#include <fstream>

#ifdef _WIN32
#include <windows.h>
    #include <psapi.h>
#elif defined(__ANDROID__)
#include <SDL3/SDL.h>
#include <unistd.h>
#include <sys/resource.h>
#include <sys/time.h>
#endif

class MetricsManager {
private:
    static MetricsManager* P_SHARED_INSTANCE;

    // FPS tracking
    unsigned long frameCount = 0;
    float fps = 0.0f;

#ifdef _WIN32
    LARGE_INTEGER qpcLastTime{};
    double qpcFrequency = 0.0;
#elif defined(__ANDROID__)
    Uint64 lastTime = 0;
#endif

    // CPU tracking
    bool cpuInitialized = false;
    double cpuUsage = 0.0;

#ifdef _WIN32
    FILETIME prevSysKernel{}, prevSysUser{};
    FILETIME prevProcKernel{}, prevProcUser{};
#elif defined(__ANDROID__)
    Uint64 lastCPUTime = 0;
#endif

    // Memory tracking
#ifdef _WIN32
    SIZE_T memoryUsage = 0;
#elif defined(__ANDROID__)
    size_t memoryUsage = 0;
#endif

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
    size_t getMemoryUsage() const { return memoryUsage; }

    // Singleton
    static void initialize();
    static void destroy();
    static MetricsManager* getInstance();

private:
    MetricsManager() {}
    ~MetricsManager() {}
};
