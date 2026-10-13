#pragma once

// Cross-platform MetricsManager header
//
// Collects per-frame performance metrics, Unity-Profiler style:
//   - CPU frame time (ms)  -> wall-clock time between update() calls (main-thread frame time)
//   - GPU frame time (ms)  -> measured internally by timing your present/swap call: wrap
//                             beginGPUFrame() / endGPUFrame() directly around SDL_RenderPresent()
//                             (or whatever call flips the frame). This is NOT a true isolated
//                             GPU-only timestamp query (that would require reaching into whatever
//                             graphics API SDL_Renderer picked underneath, which isn't reliably
//                             accessible when only plain SDL3 is used). It's a lightweight, zero-
//                             dependency proxy: when the frame is GPU-bound, the present call
//                             takes measurably longer (driver/vsync/swapchain wait), so timing it
//                             gives a reasonable per-frame GPU-cost signal without issuing any
//                             queries, touching any graphics API, or adding any per-frame overhead
//                             beyond two clock reads. If you later gain direct API access (a GL
//                             context, a Vulkan device, etc.), setGPUFrameTime() is still available
//                             to feed in a real timer-query result instead.
//   - FPS                  -> instantaneous (smoothed) + running min / max / average
//   - Memory usage        -> app-owned memory footprint, NOT total RSS: private bytes (Windows) /
//                            RssAnon + RssShmem + VmSwap (Android & other Linux-based). RSS also counts
//                            file-backed mapped pages (libs, assets, driver caches) which vary wildly
//                            between devices and ROMs and are reclaimable, so it is reported separately
//                            as RssMB and is not used for the main "Memory" number.
//
// Every update() call buffers one row of the above into memory; the buffer is auto-flushed to a
// single CSV file periodically (see setAutoFlushFrameCount), on destroy(), and on demand via
// exportCSV(). This keeps per-frame logging cheap without doing a disk write every single frame.
//
// On Windows: uses QueryPerformanceCounter, GetProcessTimes/GetSystemTimes, GetProcessMemoryInfo.
// On other platforms (Android/Linux-based): uses std::chrono, clock_gettime(CLOCK_PROCESS_CPUTIME_ID)
// and /proc/self/status (+ mallinfo) - all available to a normal (non-rooted) process on Android,
// no JNI required. (/proc/stat is NOT used: Android 8+ blocks it for apps, so CPU% read as 0.)
//
// IMPORTANT (Android CSV path): a native Android process has no writable relative working
// directory, so the default "metrics_export.csv" will silently fail to open there. Call
// setCSVPath() once, right after initialize(), with an absolute path obtained from Java/Kotlin:
//
//   // Java/Kotlin side, e.g. in onCreate():
//   String metricsDir = getExternalFilesDir(null).getAbsolutePath();
//   nativeSetMetricsPath(metricsDir + "/metrics_export.csv"); // your own JNI bridge method
//
//   // C++ side, in the JNI function you expose for nativeSetMetricsPath:
//   MetricsManager::getInstance()->setCSVPath(pathFromJava);
//
// getExternalFilesDir(null) requires no special permission and is visible over adb / a file
// manager for debugging, but is wiped on uninstall; use getFilesDir() instead for an internal,
// non-user-visible location if that's preferred.

#if defined(_WIN32) || defined(_WIN64)

#define NOMINMAX
#include <windows.h>
#include <psapi.h>

#include <string>
#include <iostream>
#include <fstream>
#include <cfloat>
#include <vector>

class MetricsManager {
private:
    static MetricsManager* P_SHARED_INSTANCE;

    // one buffered row of per-frame data, written out to CSV
    struct FrameRecord {
        unsigned long long frameIndex = 0;
        double timeSec = 0.0;
        float fps = 0.0f;
        float avgFPS = 0.0f;
        float minFPS = 0.0f;
        float maxFPS = 0.0f;
        double cpuFrameTimeMs = 0.0;
        double gpuFrameTimeMs = 0.0;
        double cpuUsagePercent = 0.0;
        double memoryMB = 0.0;
        double anonMB = 0.0;        // Linux/Android: RssAnon (heap/stack). Windows: private bytes
        double fileMB = 0.0;        // Linux/Android: RssFile (mapped libs/assets). Windows: 0
        double shmemMB = 0.0;       // Linux/Android: RssShmem (shared/GPU-driver buffers). Windows: 0
        double nativeHeapMB = 0.0;  // Linux/Android: malloc heap in use (mallinfo). Windows: 0
        double rssMB = 0.0;         // total RSS / working set (includes file-backed pages)
    };

    // FPS tracking
    unsigned long long frameCount = 0;
    float fps = 0.0f;
    LARGE_INTEGER qpcLastFrameTime{};
    LARGE_INTEGER qpcAppStart{};
    double qpcFrequency = 0.0;

    // FPS stats
    float avgFPS = 0.0f;
    float minFPS = FLT_MAX;
    float maxFPS = 0.0f;

    // FPS smoothing
    const double fpsSmoothingAlpha = 0.12; // 0..1 (0 = no smoothing, 1 = instant)

    // CPU frame time (ms) - wall time between update() calls, Unity-profiler "CPU" style
    double cpuFrameTimeMs = 0.0;

    // GPU frame time (ms) - measured internally by timing the present/swap call, see
    // beginGPUFrame()/endGPUFrame(); setGPUFrameTime() remains available as a manual override.
    double gpuFrameTimeMs = 0.0;
    LARGE_INTEGER qpcGpuFrameStart{};
    bool gpuFrameTimingActive = false;

    // Process-wide CPU usage percent (bonus stat alongside frame time), FILETIME based
    FILETIME prevSysKernel{}, prevSysUser{};
    FILETIME prevProcKernel{}, prevProcUser{};
    bool cpuInitialized = false;
    double cpuUsagePercent = 0.0; // percent of total machine capacity (0..100)
    const double cpuSmoothingAlpha = 0.15;
    unsigned int logicalProcessorCount = 1;

    // Memory tracking
    SIZE_T memoryUsage = 0; // bytes (working set)
    size_t memAnonBytes = 0;       // private bytes (PrivateUsage)
    size_t memFileBytes = 0;       // unused on Windows
    size_t memShmemBytes = 0;      // unused on Windows
    size_t memNativeHeapBytes = 0; // unused on Windows
    size_t memFootprintBytes = 0;  // reported "Memory": private bytes

    // Load Time tracking
    double loadTimeSec = 0.0;
    LARGE_INTEGER loadStart{};

    // Input lag tracking
    double inputLagMs = 0.0;
    LONGLONG inputTimestamp = 0;

    // Toggles
    bool showFPS = true;
    bool showCPU = true;
    bool showMemory = true;
    bool showGPU = true;
    bool showInputLag = true;
    bool showLoadTime = true;

    // History buffers (for GUI plots only)
    static const int HISTORY_SIZE = 100;
    float fpsHistory[HISTORY_SIZE] = {};
    float cpuHistory[HISTORY_SIZE] = {};
    int offset = 0;
    int historyCount = 0;

    // Per-frame CSV log buffer
    std::vector<FrameRecord> frameLog;
    size_t autoFlushFrameCount = 300; // ~5s at 60fps; auto-flush buffer to disk at this size
    std::string csvFilename = "metrics_export.csv";
    bool csvHeaderWritten = false;

    // If a flush fails (bad path, locked file, ...) rows stay buffered. Retry only every
    // autoFlushFrameCount frames instead of every frame, and cap the buffer so a permanently
    // failing path can never turn into unbounded memory growth.
    size_t nextFlushRetryFrame = 0;
    static constexpr size_t kMaxBufferedRows = 5000;

    void appendFrameRecord();
    void flushToDisk(const std::string& filename);

public:
    void update();
    void logMetrics();
    void drawGUI();

    // Sets the path future CSV rows (auto-flush and exportCSV()) are written to, WITHOUT writing
    // anything immediately. Call this once, right after initialize(), before any frames run -
    // on Android there is no usable relative working directory, so you must pass an absolute
    // path from Java/Kotlin (Context.getExternalFilesDir(null) or getFilesDir()) down via JNI.
    // On Windows this is optional; without it, the relative default resolves against whatever
    // the process's current working directory happens to be at launch.
    void setCSVPath(const std::string& path) { csvFilename = path; csvHeaderWritten = false; nextFlushRetryFrame = 0; }

    // Flushes any buffered per-frame rows to `filename` (creates the file / header if needed).
    // Pass nothing (or "") to flush to whatever path is already configured (via setCSVPath(),
    // or the last exportCSV() call) instead of resetting it back to the relative default -
    // calling exportCSV() with no args is safe and will NOT clobber a path you set earlier.
    // Also called automatically every `autoFlushFrameCount` frames and on destroy().
    void exportCSV(const std::string& filename = "");

    // Convenience alias for exportCSV() with no args - flushes now, to the current path.
    // On Android, call this from your JNI bridge for onPause()/onStop(), since the OS can kill
    // the process at any time without your native destructors running, silently dropping
    // whatever hasn't hit autoFlushFrameCount yet.
    void flush() { exportCSV(); }

    void setAutoFlushFrameCount(size_t frames) { autoFlushFrameCount = frames; }

    // getters
    float getFPS() const { return fps; }
    float getAvgFPS() const { return avgFPS; }
    float getMinFPS() const { return minFPS; }
    float getMaxFPS() const { return maxFPS; }
    double getCPUFrameTime() const { return cpuFrameTimeMs; } // ms
    double getGPUFrameTime() const { return gpuFrameTimeMs; } // ms
    double getCPUUsagePercent() const { return cpuUsagePercent; }
    SIZE_T getMemoryUsage() const { return (SIZE_T)memFootprintBytes; } // bytes, app-owned footprint
    SIZE_T getRssUsage() const { return memoryUsage; }                  // bytes, working set
    double getInputLag() const { return inputLagMs; }
    double getLoadTime() const { return loadTimeSec; }

    // Wrap these directly around your present/swap call (e.g. SDL_RenderPresent()) to measure
    // GPU frame time internally, once per frame - see header comment above for what this measures.
    void beginGPUFrame();
    void endGPUFrame();

    // Manual override: feed in a real timer-query result instead, if you ever get direct
    // graphics-API access. Whichever was called most recently (this or endGPUFrame()) wins.
    void setGPUFrameTime(double ms) { gpuFrameTimeMs = ms; }

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

#else // ---------------- non-Windows (Android / other Linux-based) fallback ----------------

#include <chrono>
#include <string>
#include <iostream>
#include <fstream>
#include <cfloat>
#include <vector>
#include <mutex>

class MetricsManager {
private:
    struct FrameRecord {
        unsigned long long frameIndex = 0;
        double timeSec = 0.0;
        float fps = 0.0f;
        float avgFPS = 0.0f;
        float minFPS = 0.0f;
        float maxFPS = 0.0f;
        double cpuFrameTimeMs = 0.0;
        double gpuFrameTimeMs = 0.0;
        double cpuUsagePercent = 0.0;
        double memoryMB = 0.0;
        double anonMB = 0.0;        // Linux/Android: RssAnon (heap/stack). Windows: private bytes
        double fileMB = 0.0;        // Linux/Android: RssFile (mapped libs/assets). Windows: 0
        double shmemMB = 0.0;       // Linux/Android: RssShmem (shared/GPU-driver buffers). Windows: 0
        double nativeHeapMB = 0.0;  // Linux/Android: malloc heap in use (mallinfo). Windows: 0
        double rssMB = 0.0;         // total RSS / working set (includes file-backed pages)
    };

    static MetricsManager* P_SHARED_INSTANCE;

    // FPS tracking
    unsigned long long frameCount = 0;
    float fps = 0.0f;
    std::chrono::steady_clock::time_point lastFrameTime;
    std::chrono::steady_clock::time_point appStartTime;

    // FPS stats
    float avgFPS = 0.0f;
    float minFPS = FLT_MAX;
    float maxFPS = 0.0f;
    const double fpsSmoothingAlpha = 0.12;

    // CPU frame time (ms) - wall time between update() calls
    double cpuFrameTimeMs = 0.0;

    // GPU frame time (ms) - measured internally by timing the present/swap call, see
    // beginGPUFrame()/endGPUFrame(); setGPUFrameTime() remains available as a manual override.
    double gpuFrameTimeMs = 0.0;
    std::chrono::steady_clock::time_point gpuFrameStart;
    bool gpuFrameTimingActive = false;

    // Process CPU usage percent (bonus stat): delta of process CPU time (all threads) over the
    // wall-clock delta, divided by core count = percent of total machine capacity.
    long long prevProcCpuNs = 0;
    long long prevWallNs = 0;
    bool cpuInitialized = false;
    double cpuUsagePercent = 0.0;
    const double cpuSmoothingAlpha = 0.3; // applied per /proc sample, not per frame
    unsigned int logicalProcessorCount = 1;

    // CPU%/memory are sampled at this interval instead of every frame (fewer file reads,
    // less measurement overhead, steadier CPU% numbers)
    const double procSampleIntervalSec = 0.25;
    std::chrono::steady_clock::time_point lastProcSample;
    bool procSampledOnce = false;

    // Memory tracking (bytes) - from /proc/self/status (+ mallinfo for the native heap)
    size_t memoryUsage = 0;         // VmRSS (total)
    size_t memAnonBytes = 0;        // RssAnon  - heap / stacks / anonymous mappings
    size_t memFileBytes = 0;        // RssFile  - mapped libraries / files
    size_t memShmemBytes = 0;       // RssShmem - shared memory (often graphics buffers)
    size_t memNativeHeapBytes = 0;  // malloc heap in use (your C/C++ allocations)
    size_t memSwapBytes = 0;        // VmSwap - anon pages pushed out to zRAM/swap
    size_t memFootprintBytes = 0;   // reported "Memory": RssAnon + RssShmem + VmSwap (excludes file-backed)

    // Load/time tracking
    double loadTimeSec = 0.0;
    std::chrono::steady_clock::time_point loadStart;

    // Input lag
    double inputLagMs = 0.0;
    long long inputTimestamp = 0;

    // History buffers (for GUI plots only)
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

    // Per-frame CSV log buffer
    std::vector<FrameRecord> frameLog;
    size_t autoFlushFrameCount = 300;
    std::string csvFilename = "metrics_export.csv";
    bool csvHeaderWritten = false;

    // If a flush fails (bad path, locked file, ...) rows stay buffered. Retry only every
    // autoFlushFrameCount frames instead of every frame, and cap the buffer so a permanently
    // failing path can never turn into unbounded memory growth.
    size_t nextFlushRetryFrame = 0;
    static constexpr size_t kMaxBufferedRows = 5000;

    void appendFrameRecord();
    void flushToDisk(const std::string& filename);
    void updateProcessCpuAndMemory(); // process CPU time, /proc/self/status, mallinfo

public:
    void update();               // call every frame
    void logMetrics();           // dump simple metrics to stdout
    void drawGUI();              // optional, stub so imgui calls compile

    // Sets the path future CSV rows (auto-flush and exportCSV()) are written to, WITHOUT writing
    // anything immediately. Call this once, right after initialize(), before any frames run.
    // On Android there is no usable relative working directory (the process's cwd is typically
    // "/", which your app cannot write to), so you MUST pass an absolute path obtained from
    // Java/Kotlin via JNI - see the usage note below.
    void setCSVPath(const std::string& path) { csvFilename = path; csvHeaderWritten = false; nextFlushRetryFrame = 0; }

    // Flushes any buffered per-frame rows to `filename` (creates the file / header if needed).
    // Pass nothing (or "") to flush to whatever path is already configured (via setCSVPath(),
    // or the last exportCSV() call) instead of resetting it back to the relative default -
    // calling exportCSV() with no args is safe and will NOT clobber a path you set earlier.
    void exportCSV(const std::string& filename = "");

    // Convenience alias for exportCSV() with no args - flushes now, to the current path.
    // Call this from your JNI bridge for onPause()/onStop(), since Android can kill the
    // process at any time without your native destructors running, silently dropping
    // whatever hasn't hit autoFlushFrameCount yet.
    void flush() { exportCSV(); }

    void setAutoFlushFrameCount(size_t frames) { autoFlushFrameCount = frames; }

    // getters
    float getFPS() const { return fps; }
    float getAvgFPS() const { return avgFPS; }
    float getMinFPS() const { return minFPS; }
    float getMaxFPS() const { return maxFPS; }
    double getCPUFrameTime() const { return cpuFrameTimeMs; }
    double getGPUFrameTime() const { return gpuFrameTimeMs; }
    double getCPUUsagePercent() const { return cpuUsagePercent; }
    size_t getMemoryUsage() const { return memFootprintBytes; } // bytes, app-owned footprint
    size_t getRssUsage() const { return memoryUsage; }          // bytes, total VmRSS incl. file-backed
    double getInputLag() const { return inputLagMs; }
    double getLoadTime() const { return loadTimeSec; }

    // Wrap these directly around your present/swap call (e.g. SDL_RenderPresent()) to measure
    // GPU frame time internally, once per frame - see header comment above for what this measures.
    void beginGPUFrame();
    void endGPUFrame();

    // Manual override: feed in a real timer-query result instead, if you ever get direct
    // graphics-API access. Whichever was called most recently (this or endGPUFrame()) wins.
    void setGPUFrameTime(double ms) { gpuFrameTimeMs = ms; }

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