#pragma once

// Cross-platform MetricsManager header
// On Windows: keep original implementation (uses PDH, FILETIME, etc.)
// On other platforms (Android): use a minimal fallback implementation
// that provides the same public interface (FPS, timers, stubs for others).

#if defined(_WIN32) || defined(_WIN64)

#define NOMINMAX
#ifdef _WIN32
#include <windows.h>
#include <psapi.h>
#include <pdh.h>
#include <tlhelp32.h>
#endif


#include <string>
#include <iostream>
#include <fstream>
#include <cfloat>
#include <thread>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <algorithm>

class MetricsManager {
private:
    static MetricsManager* P_SHARED_INSTANCE;

    // FPS tracking
    unsigned long frameCount = 0;
    float fps = 0.0f;
    LARGE_INTEGER qpcLastTime{};          // 1s bucket reference (optional)
    LARGE_INTEGER qpcLastFrameTime{};     // per-frame last time
    LARGE_INTEGER qpcLastThreadSampleTime{}; // last time threads were sampled
    double qpcFrequency = 0.0;

    // FPS stats
    float avgFPS = 0.0f;
    float minFPS = FLT_MAX;
    float maxFPS = 0.0f;

    // FPS smoothing
    const double fpsSmoothingAlpha = 0.12; // 0..1 (0 = no smoothing, 1 = instant)

    // Thread sampling throttle (seconds)
    double threadSampleIntervalSec = 0.20; // default 200 ms

    // CPU tracking (process-wide FILETIME based)
    FILETIME prevSysIdle{}, prevSysKernel{}, prevSysUser{};
    FILETIME prevProcKernel{}, prevProcUser{};
    bool cpuInitialized = false;
    double cpuUsage = 0.0;          // percent of total machine capacity (0..100)
    double cpuUsagePerCore = 0.0;   // scaled so 100 = one logical core fully used
    unsigned int logicalProcessorCount = 1;

    // optional smoothing for CPU
    const double cpuSmoothingAlpha = 0.15; // small smoothing to avoid jitter

    // PDH per-core (optional/system-wide)
    PDH_HQUERY hPdhQuery = NULL;
    std::vector<PDH_HCOUNTER> coreCounters;
    std::vector<float> corePercentages;
    std::vector<std::vector<float>> coreHistory; // per-core ring buffers
    bool pdhInitialized = false;

    // Per-thread (app-only) CPU tracking
    struct ThreadStat {
        DWORD tid = 0;
        unsigned long long prevTotal = 0; // FILETIME kernel+user (100ns units)
        float usage = 0.0f;               // percent (100% ~= one core)
        std::vector<float> history;       // HISTORY_SIZE entries
    };
    std::unordered_map<DWORD, ThreadStat> threadStats; // keyed by TID
    bool threadStatsInitialized = false;

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
    bool showThreads = true; // toggle thread view

    // History buffers
    static const int HISTORY_SIZE = 100;
    float fpsHistory[HISTORY_SIZE] = {};
    float cpuHistory[HISTORY_SIZE] = {};
    int offset = 0;
    int historyCount = 0; // how many entries we've filled (<= HISTORY_SIZE)

    // PDH helpers
    bool initPDH();
    void cleanupPDH();
    void updatePDH(); // reads per-core counters and writes into corePercentages & coreHistory[][offset]

    // Thread helpers
    void updateThreadStats(double sampleIntervalSeconds); // updates threadStats and per-thread histories
    void pruneDeadThreads(const std::vector<DWORD>& liveTids); // removes entries no longer present

public:
    void update();
    void logMetrics();
    void drawGUI();
    void exportCSV(const std::string& filename = "metrics_export.csv");

    // getters
    float getFPS() const { return fps; }
    double getCPUUsage() const { return cpuUsage; } // total percent
    double getCPUUsagePerCore() const { return cpuUsagePerCore; } // per-core scaled percent
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

#else // non-Windows: minimal cross-platform (Android-friendly) fallback

#include <chrono>
#include <string>
#include <iostream>
#include <fstream>
#include <cfloat>
#include <vector>
#include <mutex>

class MetricsManager {
private:
    static MetricsManager* P_SHARED_INSTANCE;

    // FPS tracking
    unsigned long frameCount = 0;
    float fps = 0.0f;

    // simple timing using chrono
    std::chrono::steady_clock::time_point lastFrameTime;
    std::chrono::steady_clock::time_point lastSampleTime;

    // FPS stats
    float avgFPS = 0.0f;
    float minFPS = FLT_MAX;
    float maxFPS = 0.0f;

    const double fpsSmoothingAlpha = 0.12;

    // minimal memory/cpu placeholders (platform-specific implementations can replace)
    size_t memoryUsage = 0;
    double cpuUsage = 0.0;
    double gpuUsage = 0.0;

    // Load/time tracking
    double loadTimeSec = 0.0;
    std::chrono::steady_clock::time_point loadStart;

    // Input lag
    double inputLagMs = 0.0;
    long long inputTimestamp = 0;

    // History buffers
    static const int HISTORY_SIZE = 100;
    float fpsHistory[HISTORY_SIZE] = {};
    float cpuHistory[HISTORY_SIZE] = {};
    int offset = 0;
    int historyCount = 0;

    // concurrency
    std::mutex mtx;

    // UI toggles
    bool showFPS      = true;
    bool showCPU      = true;
    bool showMemory   = true;
    bool showGPU      = true;
    bool showInputLag = true;
    bool showLoadTime = true;
    bool showThreads  = true;

public:
    // Basic lifecycle & operations (lightweight)
    void update();               // call every frame
    void logMetrics();           // dump simple metrics to stdout
    void drawGUI();              // optional, stub so imgui calls compile
    void exportCSV(const std::string& filename = "metrics_export.csv");

    // getters
    float getFPS() const { return fps; }
    double getCPUUsage() const { return cpuUsage; }
    double getCPUUsagePerCore() const { return cpuUsage; }
    size_t getMemoryUsage() const { return memoryUsage; }
    double getGPUUsage() const { return gpuUsage; }
    double getInputLag() const { return inputLagMs; }
    double getLoadTime() const { return loadTimeSec; }

    // load time helpers
    void startLoadTimer();
    void endLoadTimer();

    // input lag helpers
    void recordInputEvent();
    void markInputHandled();

    // singleton
    static void initialize();
    static void destroy();
    static MetricsManager* getInstance();

private:
    MetricsManager();
    ~MetricsManager();
};

#endif // _WIN32