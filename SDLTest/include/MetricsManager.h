#pragma once

// Cross-platform metrics manager header
// - Windows-only includes / members are guarded by _WIN32
// - POSIX / Android uses portable types (clock_gettime, /proc, sysconf)

#include <string>
#include <iostream>
#include <fstream>
#include <cfloat>
#include <thread>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <cstdint>
#include <cstddef>

#ifdef _WIN32
#ifndef NOMINMAX
  #define NOMINMAX
  #endif
  #include <windows.h>
  #include <psapi.h>
  #include <pdh.h>
  #include <tlhelp32.h>
#else
// POSIX / Android includes
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#endif

class MetricsManager {
private:
    static MetricsManager* P_SHARED_INSTANCE;

    // FPS tracking
    unsigned long frameCount = 0;
    float fps = 0.0f;

#ifdef _WIN32
    // Windows high-resolution counters
    LARGE_INTEGER qpcLastTime{};          // 1s bucket reference (optional)
    LARGE_INTEGER qpcLastFrameTime{};     // per-frame last time
    LARGE_INTEGER qpcLastThreadSampleTime{}; // last time threads were sampled
    double qpcFrequency = 0.0;
#else
    // POSIX: store times in nanoseconds (CLOCK_MONOTONIC)
    int64_t qpcLastTimeNs = 0;
    int64_t qpcLastFrameTimeNs = 0;
    int64_t qpcLastThreadSampleTimeNs = 0;
    double qpcFrequency = 1.0;
#endif

    // FPS stats
    float avgFPS = 0.0f;
    float minFPS = FLT_MAX;
    float maxFPS = 0.0f;

    // FPS smoothing
    const double fpsSmoothingAlpha = 0.12; // 0..1 (0 = no smoothing, 1 = instant)

    // Thread sampling throttle (seconds)
    double threadSampleIntervalSec = 0.20; // default 200 ms

    // CPU tracking (process-wide):
#ifdef _WIN32
    // FILETIME based approach (Windows)
    FILETIME prevSysIdle{}, prevSysKernel{}, prevSysUser{};
    FILETIME prevProcKernel{}, prevProcUser{};
    bool cpuInitialized = false;
#else
    // POSIX: store previous total/idle counters (in jiffies or nanoseconds) as uint64_t
    uint64_t prevTotalJiffies = 0;
    uint64_t prevIdleJiffies = 0;
    bool cpuInitialized = false;
#endif
    double cpuUsage = 0.0;          // percent of total machine capacity (0..100)
    double cpuUsagePerCore = 0.0;   // scaled so 100 = one logical core fully used
    unsigned int logicalProcessorCount = 1;

    const double cpuSmoothingAlpha = 0.15; // small smoothing to avoid jitter

    // PDH per-core (Windows-only; stubbed out on POSIX)
#ifdef _WIN32
    PDH_HQUERY hPdhQuery = NULL;
    std::vector<PDH_HCOUNTER> coreCounters;
#endif
    std::vector<float> corePercentages;
    std::vector<std::vector<float>> coreHistory; // per-core ring buffers
    bool pdhInitialized = false;

    // Per-thread (app-only) CPU tracking
    struct ThreadStat {
#ifdef _WIN32
        DWORD tid = 0;
#else
        uint32_t tid = 0;
#endif
        unsigned long long prevTotal = 0;
        float usage = 0.0f;               // percent (100% ~= one core)
        std::vector<float> history;       // HISTORY_SIZE entries
    };
    std::unordered_map<uint32_t, ThreadStat> threadStats; // keyed by TID
    bool threadStatsInitialized = false;

    // Memory tracking (portable)
    size_t memoryUsage = 0;

    // GPU tracking (external only, set 0 here)
    double gpuUsage = 0.0;

    // Load Time tracking
    double loadTimeSec = 0.0;
#ifdef _WIN32
    LARGE_INTEGER loadStart{};
#else
    int64_t loadStartNs = 0;
#endif

    // INPUT Lag tracking
    double inputLagMs = 0.0;
#ifdef _WIN32
    LONGLONG inputTimestamp = 0;
#else
    int64_t inputTimestampNs = 0;
#endif

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

    // PDH helpers (Windows-only)
#ifdef _WIN32
    bool initPDH();
    void cleanupPDH();
    void updatePDH(); // reads per-core counters and writes into corePercentages & coreHistory[][offset]
#else
    // On POSIX/Android these are no-ops / stubs you must implement in the .cpp
    bool initPDH() { return false; }
    void cleanupPDH() {}
    void updatePDH() {}
#endif

    // Thread helpers (POSIX implementation will inspect /proc/self/task)
    void updateThreadStats(double sampleIntervalSeconds); // updates threadStats and per-thread histories
    void pruneDeadThreads(const std::vector<uint32_t>& liveTids); // removes entries no longer present

public:
    void update();
    void logMetrics();
    void drawGUI();
    void exportCSV(const std::string& filename = "metrics_export.csv");

    // getters
    float getFPS() const { return fps; }
    double getCPUUsage() const { return cpuUsage; } // total percent
    double getCPUUsagePerCore() const { return cpuUsagePerCore; } // per-core scaled percent
    size_t getMemoryUsage() const { return memoryUsage; }
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