// MetricsManager.cpp
#include "MetricsManager.h"
#include "imgui.h"
#include <iostream>
#include <sstream>

#if !defined(_WIN32) && !defined(_WIN64)
#include <chrono>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <mutex>
#include <unistd.h>
#if defined(__ANDROID__)
#include <android/log.h>
#endif
#endif

// Logging helpers: on Android, plain std::cout/std::cerr from native code typically never reach
// logcat (there's no terminal attached), so a write failure would otherwise be completely
// invisible - route through __android_log_print there instead. Elsewhere, stdout/stderr are fine.
namespace {
    void mmLogInfo(const std::string& msg) {
#if defined(__ANDROID__)
        __android_log_print(ANDROID_LOG_INFO, "MetricsManager", "%s", msg.c_str());
#else
        std::cout << msg << std::endl;
#endif
    }
    void mmLogError(const std::string& msg) {
#if defined(__ANDROID__)
        __android_log_print(ANDROID_LOG_ERROR, "MetricsManager", "%s", msg.c_str());
#else
        std::cerr << msg << std::endl;
#endif
    }
}

// single definition for the singleton pointer (applies to both branches)
MetricsManager* MetricsManager::P_SHARED_INSTANCE = nullptr;

#if defined(_WIN32) || defined(_WIN64)

/* ---------------- Windows implementation ---------------- */

static inline unsigned long long FileTimeToULL(const FILETIME& ft) {
    return (((unsigned long long)ft.dwHighDateTime) << 32) | ft.dwLowDateTime;
}

/* ---------------- Initialization / destroy ---------------- */
void MetricsManager::initialize() {
    if (!P_SHARED_INSTANCE) {
        P_SHARED_INSTANCE = new MetricsManager();
        std::cout << "[MetricsManager] Initialized." << std::endl;

        // QPC init
        LARGE_INTEGER freq;
        if (!QueryPerformanceFrequency(&freq) || freq.QuadPart == 0) {
            std::cerr << "[MetricsManager] QueryPerformanceFrequency failed, using fallback." << std::endl;
            P_SHARED_INSTANCE->qpcFrequency = 1.0;
        }
        else {
            P_SHARED_INSTANCE->qpcFrequency = double(freq.QuadPart);
        }
        QueryPerformanceCounter(&P_SHARED_INSTANCE->qpcLastFrameTime);
        P_SHARED_INSTANCE->qpcAppStart = P_SHARED_INSTANCE->qpcLastFrameTime;

        // logical CPUs
        SYSTEM_INFO si;
        GetSystemInfo(&si);
        P_SHARED_INSTANCE->logicalProcessorCount = si.dwNumberOfProcessors ? si.dwNumberOfProcessors : 1u;
        std::cout << "[MetricsManager] logical processors: " << P_SHARED_INSTANCE->logicalProcessorCount << std::endl;
    }
}

void MetricsManager::destroy() {
    if (P_SHARED_INSTANCE) {
        // flush any buffered per-frame rows before tearing down
        P_SHARED_INSTANCE->flushToDisk(P_SHARED_INSTANCE->csvFilename);
        delete P_SHARED_INSTANCE;
        P_SHARED_INSTANCE = nullptr;
        std::cout << "[MetricsManager] Destroyed." << std::endl;
    }
}

MetricsManager* MetricsManager::getInstance() {
    return P_SHARED_INSTANCE;
}

/* ---------------- Main update ---------------- */
void MetricsManager::update() {
    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);

    // --- CPU frame time (wall time since last update() call) ---
    double frameDelta = double(now.QuadPart - qpcLastFrameTime.QuadPart) / qpcFrequency;
    if (frameDelta <= 0.0) frameDelta = 1.0 / 1000.0; // defensive small delta
    cpuFrameTimeMs = frameDelta * 1000.0;

    // --- FPS (instantaneous -> EMA-smoothed) + min/max from the raw instantaneous value ---
    double instFps = (frameDelta > 0.0) ? (1.0 / frameDelta) : 0.0;
    fps = float((1.0 - fpsSmoothingAlpha) * (double)fps + fpsSmoothingAlpha * instFps);

    frameCount++;
    if (frameCount > 1) { // skip the very first sample (startup transient)
        float instFpsF = float(instFps);
        if (instFpsF < minFPS) minFPS = instFpsF;
        if (instFpsF > maxFPS) maxFPS = instFpsF;
    }

    // --- Process-wide CPU % via FILETIME (bonus stat alongside cpuFrameTimeMs) ---
    FILETIME sysIdle, sysKernel, sysUser;
    FILETIME procCreation, procExit, procKernel, procUser;

    if (GetSystemTimes(&sysIdle, &sysKernel, &sysUser) &&
        GetProcessTimes(GetCurrentProcess(), &procCreation, &procExit, &procKernel, &procUser)) {

        if (cpuInitialized) {
            unsigned long long sysKernelDiff = FileTimeToULL(sysKernel) - FileTimeToULL(prevSysKernel);
            unsigned long long sysUserDiff = FileTimeToULL(sysUser) - FileTimeToULL(prevSysUser);

            unsigned long long procKernelDiff = FileTimeToULL(procKernel) - FileTimeToULL(prevProcKernel);
            unsigned long long procUserDiff = FileTimeToULL(procUser) - FileTimeToULL(prevProcUser);

            unsigned long long sysTotal = sysKernelDiff + sysUserDiff;
            unsigned long long procTotal = procKernelDiff + procUserDiff;

            double cpuPctTotal = 0.0;
            if (sysTotal > 0) cpuPctTotal = (double)procTotal * 100.0 / (double)sysTotal;

            cpuUsagePercent = (1.0 - cpuSmoothingAlpha) * cpuUsagePercent + cpuSmoothingAlpha * cpuPctTotal;
        }
        else {
            cpuUsagePercent = 0.0;
        }

        prevSysKernel = sysKernel;
        prevSysUser = sysUser;
        prevProcKernel = procKernel;
        prevProcUser = procUser;
        cpuInitialized = true;
    }

    // --- Memory ---
    PROCESS_MEMORY_COUNTERS_EX pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc))) {
        memoryUsage = pmc.WorkingSetSize;
    }

    // --- history buffers (GUI plots) ---
    fpsHistory[offset] = fps;
    cpuHistory[offset] = (float)cpuUsagePercent;
    offset = (offset + 1) % HISTORY_SIZE;
    if (historyCount < HISTORY_SIZE) ++historyCount;

    float sum = 0.0f;
    for (int i = 0; i < historyCount; ++i) sum += fpsHistory[i];
    avgFPS = (historyCount > 0) ? (sum / historyCount) : fps;

    // --- buffer this frame's row for CSV export ---
    appendFrameRecord();

    qpcLastFrameTime = now;
}

/* ---------------- Logging & GUI ---------------- */
void MetricsManager::logMetrics() {
    std::cout << "[Metrics] FPS=" << fps
        << " | AvgFPS=" << avgFPS
        << " | MinFPS=" << minFPS
        << " | MaxFPS=" << maxFPS
        << " | CPUFrameTime(ms)=" << cpuFrameTimeMs
        << " | GPUFrameTime(ms)=" << gpuFrameTimeMs
        << " | CPU(total%)=" << cpuUsagePercent
        << " | Mem=" << (memoryUsage / 1024.0 / 1024.0) << " MB"
        << std::endl;
}

void MetricsManager::drawGUI() {
    ImGui::Begin("Performance Metrics");

    ImGui::Checkbox("Show FPS", &showFPS);
    ImGui::Checkbox("Show CPU", &showCPU);
    ImGui::Checkbox("Show Memory", &showMemory);
    ImGui::Checkbox("Show GPU", &showGPU);
    ImGui::Checkbox("Show Input Lag", &showInputLag);
    ImGui::Checkbox("Show Load Time", &showLoadTime);

    if (ImGui::BeginTabBar("MetricsTabs")) {
        if (showFPS && ImGui::BeginTabItem("FPS")) {
            ImGui::Text("FPS: %.1f", fps);
            ImGui::Text("Avg FPS: %.1f", avgFPS);
            ImGui::Text("Min FPS: %.1f", minFPS);
            ImGui::Text("Max FPS: %.1f", maxFPS);
            ImGui::PlotLines("FPS History", fpsHistory, HISTORY_SIZE, offset, nullptr, 0.0f, 240.0f, ImVec2(0, 80));
            ImGui::EndTabItem();
        }

        if (showCPU && ImGui::BeginTabItem("CPU")) {
            ImGui::Text("CPU Frame Time: %.3f ms", cpuFrameTimeMs);
            ImGui::Text("CPU Usage (total): %.2f %%", cpuUsagePercent);
            ImGui::PlotLines("CPU History", cpuHistory, HISTORY_SIZE, offset, nullptr, 0.0f, 100.0f, ImVec2(0, 80));
            ImGui::EndTabItem();
        }

        if (showGPU && ImGui::BeginTabItem("GPU")) {
            ImGui::Text("GPU Frame Time: %.3f ms", gpuFrameTimeMs);
            ImGui::TextDisabled("(timed via beginGPUFrame()/endGPUFrame() around present)");
            ImGui::EndTabItem();
        }

        if (showMemory && ImGui::BeginTabItem("Memory")) {
            ImGui::Text("Memory Usage: %.2f MB", memoryUsage / 1024.0 / 1024.0);
            ImGui::EndTabItem();
        }

        if (showInputLag && ImGui::BeginTabItem("Input Lag")) {
            ImGui::Text("Last Input Lag: %.2f ms", inputLagMs);
            ImGui::EndTabItem();
        }

        if (showLoadTime && ImGui::BeginTabItem("Load Time")) {
            ImGui::Text("Last Load Time: %.2f s", loadTimeSec);
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    if (ImGui::Button("Export CSV")) {
        exportCSV();
    }

    ImGui::End();
}

/* ---------------- CSV ---------------- */
void MetricsManager::appendFrameRecord() {
    FrameRecord rec;
    rec.frameIndex = frameCount;
    rec.timeSec = double(qpcLastFrameTime.QuadPart - qpcAppStart.QuadPart) / qpcFrequency;
    rec.fps = fps;
    rec.avgFPS = avgFPS;
    rec.minFPS = (minFPS == FLT_MAX) ? 0.0f : minFPS;
    rec.maxFPS = maxFPS;
    rec.cpuFrameTimeMs = cpuFrameTimeMs;
    rec.gpuFrameTimeMs = gpuFrameTimeMs;
    rec.cpuUsagePercent = cpuUsagePercent;
    rec.memoryMB = memoryUsage / 1024.0 / 1024.0;

    frameLog.push_back(rec);

    if (frameLog.size() >= autoFlushFrameCount) {
        flushToDisk(csvFilename);
    }
}

void MetricsManager::flushToDisk(const std::string& filename) {
    if (frameLog.empty()) {
        mmLogInfo("[MetricsManager] flush requested but no buffered rows for " + filename);
        return;
    }

    bool needsHeader = !csvHeaderWritten;
    if (needsHeader) {
        std::ifstream check(filename);
        needsHeader = !check.good() || check.peek() == std::ifstream::traits_type::eof();
    }

    std::ofstream out(filename, std::ios::app);
    if (!out.is_open()) {
        mmLogError("[MetricsManager] Failed to open " + filename + " for CSV export");
        return;
    }

    if (needsHeader) {
        out << "Frame,TimeSec,FPS,AvgFPS,MinFPS,MaxFPS,CPUFrameTimeMs,GPUFrameTimeMs,CPUUsagePercent,MemoryMB\n";
        csvHeaderWritten = true;
    }

    size_t rowsWritten = frameLog.size();
    for (const auto& r : frameLog) {
        out << r.frameIndex << "," << r.timeSec << "," << r.fps << "," << r.avgFPS << ","
            << r.minFPS << "," << r.maxFPS << "," << r.cpuFrameTimeMs << "," << r.gpuFrameTimeMs
            << "," << r.cpuUsagePercent << "," << r.memoryMB << "\n";
    }

    out.close();
    frameLog.clear();
    mmLogInfo("[MetricsManager] Wrote " + std::to_string(rowsWritten) + " row(s) to " + filename);
}

void MetricsManager::exportCSV(const std::string& filename) {
    if (!filename.empty()) csvFilename = filename;
    flushToDisk(csvFilename);
}

// GPU frame time (present-call timing proxy - see header comment for what this measures)
void MetricsManager::beginGPUFrame() {
    QueryPerformanceCounter(&qpcGpuFrameStart);
    gpuFrameTimingActive = true;
}
void MetricsManager::endGPUFrame() {
    if (!gpuFrameTimingActive) return;
    LARGE_INTEGER end;
    QueryPerformanceCounter(&end);
    gpuFrameTimeMs = double(end.QuadPart - qpcGpuFrameStart.QuadPart) / qpcFrequency * 1000.0;
    gpuFrameTimingActive = false;
}

// Load time
void MetricsManager::startLoadTimer() {
    QueryPerformanceCounter(&loadStart);
}
void MetricsManager::endLoadTimer() {
    LARGE_INTEGER end;
    QueryPerformanceCounter(&end);
    loadTimeSec = double(end.QuadPart - loadStart.QuadPart) / qpcFrequency;
}

// Input lag
void MetricsManager::recordInputEvent() {
    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);
    inputTimestamp = now.QuadPart;
}
void MetricsManager::markInputHandled() {
    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);
    if (inputTimestamp != 0) {
        double diff = double(now.QuadPart - inputTimestamp) / qpcFrequency;
        inputLagMs = diff * 1000.0;
        inputTimestamp = 0;
    }
}

#else // ---------------- non-Windows (Android / other Linux-based) implementation ----------------

using namespace std::chrono;

MetricsManager::MetricsManager() {
    lastFrameTime = steady_clock::now();
    appStartTime = lastFrameTime;
    offset = 0;
    historyCount = 0;

#if defined(_SC_CLK_TCK)
    long ticks = sysconf(_SC_CLK_TCK);
    clockTicksPerSec = (ticks > 0) ? ticks : 100;
#endif
#if defined(_SC_NPROCESSORS_ONLN)
    long cores = sysconf(_SC_NPROCESSORS_ONLN);
    logicalProcessorCount = (cores > 0) ? (unsigned int)cores : 1u;
#endif
}

MetricsManager::~MetricsManager() {}

void MetricsManager::initialize() {
    if (!P_SHARED_INSTANCE) {
        P_SHARED_INSTANCE = new MetricsManager();
    }
}

void MetricsManager::destroy() {
    if (P_SHARED_INSTANCE) {
        // flush any buffered per-frame rows before tearing down
        P_SHARED_INSTANCE->flushToDisk(P_SHARED_INSTANCE->csvFilename);
        delete P_SHARED_INSTANCE;
        P_SHARED_INSTANCE = nullptr;
    }
}

MetricsManager* MetricsManager::getInstance() {
    if (!P_SHARED_INSTANCE) initialize();
    return P_SHARED_INSTANCE;
}

// GPU frame time (present-call timing proxy - see header comment for what this measures)
void MetricsManager::beginGPUFrame() {
    gpuFrameStart = steady_clock::now();
    gpuFrameTimingActive = true;
}
void MetricsManager::endGPUFrame() {
    if (!gpuFrameTimingActive) return;
    auto end = steady_clock::now();
    gpuFrameTimeMs = duration<double>(end - gpuFrameStart).count() * 1000.0;
    gpuFrameTimingActive = false;
}

void MetricsManager::startLoadTimer() {
    loadStart = steady_clock::now();
}

void MetricsManager::endLoadTimer() {
    auto now = steady_clock::now();
    loadTimeSec = duration<double>(now - loadStart).count();
}

void MetricsManager::recordInputEvent() {
    inputTimestamp = duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
}

void MetricsManager::markInputHandled() {
    long long nowMs = duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
    inputLagMs = (nowMs > inputTimestamp) ? double(nowMs - inputTimestamp) : 0.0;
    inputTimestamp = 0;
}

// Reads process CPU% (via /proc/self/stat + /proc/stat) and RSS memory (via /proc/self/status).
// Both are readable by an app for its own PID on Android without any special permissions.
void MetricsManager::updateProcessCpuAndMemory() {
    // --- CPU: process ticks (utime+stime) vs system-wide ticks ---
    {
        std::ifstream statFile("/proc/self/stat");
        if (statFile.is_open()) {
            std::string line;
            std::getline(statFile, line);

            // fields after the last ')' are: state ppid pgrp session tty_nr tpgid flags
            // minflt cminflt majflt cmajflt utime stime ...
            size_t closeParen = line.find_last_of(')');
            unsigned long long utime = 0, stime = 0;
            if (closeParen != std::string::npos) {
                std::istringstream iss(line.substr(closeParen + 1));
                std::string token;
                // skip: state ppid pgrp session tty_nr tpgid flags minflt cminflt majflt cmajflt (11 tokens)
                for (int i = 0; i < 11 && (iss >> token); ++i) {}
                iss >> utime >> stime;
            }
            unsigned long long procTicks = utime + stime;

            unsigned long long sysTicks = 0;
            std::ifstream cpuStat("/proc/stat");
            if (cpuStat.is_open()) {
                std::string cpuLabel;
                unsigned long long user=0, nice=0, system=0, idle=0, iowait=0, irq=0, softirq=0, steal=0;
                cpuStat >> cpuLabel >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal;
                sysTicks = user + nice + system + idle + iowait + irq + softirq + steal;
            }

            if (cpuInitialized) {
                unsigned long long procDelta = (procTicks >= prevProcTicks) ? (procTicks - prevProcTicks) : 0ULL;
                unsigned long long sysDelta = (sysTicks >= prevSysTicks) ? (sysTicks - prevSysTicks) : 0ULL;

                double cpuPctTotal = 0.0;
                if (sysDelta > 0) cpuPctTotal = (double)procDelta * 100.0 / (double)sysDelta;

                cpuUsagePercent = (1.0 - cpuSmoothingAlpha) * cpuUsagePercent + cpuSmoothingAlpha * cpuPctTotal;
            }
            else {
                cpuUsagePercent = 0.0;
            }

            prevProcTicks = procTicks;
            prevSysTicks = sysTicks;
            cpuInitialized = true;
        }
    }

    // --- Memory: VmRSS from /proc/self/status ---
    {
        std::ifstream statusFile("/proc/self/status");
        if (statusFile.is_open()) {
            std::string line;
            while (std::getline(statusFile, line)) {
                if (line.compare(0, 6, "VmRSS:") == 0) {
                    std::istringstream iss(line.substr(6));
                    unsigned long long kb = 0;
                    iss >> kb;
                    memoryUsage = (size_t)(kb * 1024ULL);
                    break;
                }
            }
        }
    }
}

void MetricsManager::update() {
    std::lock_guard<std::mutex> lock(mtx);

    auto now = steady_clock::now();
    double dt = duration<double>(now - lastFrameTime).count();
    if (dt <= 0.0) dt = 1.0 / 1000.0;

    // --- CPU frame time (wall time since last update() call) ---
    cpuFrameTimeMs = dt * 1000.0;

    // --- FPS (instantaneous -> EMA-smoothed) + min/max from the raw instantaneous value ---
    double instFps = 1.0 / dt;
    fps = float((1.0 - fpsSmoothingAlpha) * (double)fps + fpsSmoothingAlpha * instFps);

    ++frameCount;
    if (frameCount > 1) { // skip the very first sample (startup transient)
        float instFpsF = float(instFps);
        if (instFpsF < minFPS) minFPS = instFpsF;
        if (instFpsF > maxFPS) maxFPS = instFpsF;
    }

    // --- CPU % + memory (bonus stats) ---
    updateProcessCpuAndMemory();

    // --- history buffers (GUI plots) ---
    fpsHistory[offset] = fps;
    cpuHistory[offset] = static_cast<float>(cpuUsagePercent);
    offset = (offset + 1) % HISTORY_SIZE;
    if (historyCount < HISTORY_SIZE) ++historyCount;

    float sum = 0.0f;
    for (int i = 0; i < historyCount; ++i) sum += fpsHistory[i];
    avgFPS = (historyCount > 0) ? (sum / historyCount) : fps;

    // --- buffer this frame's row for CSV export ---
    appendFrameRecord();

    lastFrameTime = now;
}

void MetricsManager::logMetrics() {
    std::ostringstream oss;
    oss << "[Metrics] FPS: " << std::fixed << std::setprecision(2) << fps
        << " avg: " << avgFPS
        << " min: " << minFPS
        << " max: " << maxFPS
        << " cpuFrameTime(ms): " << cpuFrameTimeMs
        << " gpuFrameTime(ms): " << gpuFrameTimeMs
        << " cpu%: " << cpuUsagePercent
        << " mem(MB): " << (memoryUsage / 1024.0 / 1024.0);
    mmLogInfo(oss.str());
}

void MetricsManager::drawGUI() {
    ImGui::Begin("Performance Metrics");

    ImGui::Checkbox("Show FPS", &showFPS);
    ImGui::Checkbox("Show CPU", &showCPU);
    ImGui::Checkbox("Show Memory", &showMemory);
    ImGui::Checkbox("Show GPU", &showGPU);
    ImGui::Checkbox("Show Input Lag", &showInputLag);
    ImGui::Checkbox("Show Load Time", &showLoadTime);

    if (ImGui::BeginTabBar("MetricsTabs")) {
        if (showFPS && ImGui::BeginTabItem("FPS")) {
            ImGui::Text("FPS: %.1f", fps);
            ImGui::Text("Avg FPS: %.1f", avgFPS);
            ImGui::Text("Min FPS: %.1f", minFPS);
            ImGui::Text("Max FPS: %.1f", maxFPS);
            ImGui::PlotLines("FPS History", fpsHistory, HISTORY_SIZE, offset, nullptr, 0.0f, 240.0f, ImVec2(0, 80));
            ImGui::EndTabItem();
        }

        if (showCPU && ImGui::BeginTabItem("CPU")) {
            ImGui::Text("CPU Frame Time: %.3f ms", cpuFrameTimeMs);
            ImGui::Text("CPU Usage (total): %.2f %%", cpuUsagePercent);
            ImGui::PlotLines("CPU History", cpuHistory, HISTORY_SIZE, offset, nullptr, 0.0f, 100.0f, ImVec2(0, 80));
            ImGui::EndTabItem();
        }

        if (showGPU && ImGui::BeginTabItem("GPU")) {
            ImGui::Text("GPU Frame Time: %.3f ms", gpuFrameTimeMs);
            ImGui::TextDisabled("(timed via beginGPUFrame()/endGPUFrame() around present)");
            ImGui::EndTabItem();
        }

        if (showMemory && ImGui::BeginTabItem("Memory")) {
            ImGui::Text("Memory Usage: %.2f MB", memoryUsage / 1024.0 / 1024.0);
            ImGui::EndTabItem();
        }

        if (showInputLag && ImGui::BeginTabItem("Input Lag")) {
            ImGui::Text("Last Input Lag: %.2f ms", inputLagMs);
            ImGui::EndTabItem();
        }

        if (showLoadTime && ImGui::BeginTabItem("Load Time")) {
            ImGui::Text("Last Load Time: %.2f s", loadTimeSec);
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    if (ImGui::Button("Export CSV")) exportCSV();

    ImGui::End();
}

void MetricsManager::appendFrameRecord() {
    FrameRecord rec;
    rec.frameIndex = frameCount;
    rec.timeSec = duration<double>(lastFrameTime - appStartTime).count();
    rec.fps = fps;
    rec.avgFPS = avgFPS;
    rec.minFPS = (minFPS == FLT_MAX) ? 0.0f : minFPS;
    rec.maxFPS = maxFPS;
    rec.cpuFrameTimeMs = cpuFrameTimeMs;
    rec.gpuFrameTimeMs = gpuFrameTimeMs;
    rec.cpuUsagePercent = cpuUsagePercent;
    rec.memoryMB = memoryUsage / 1024.0 / 1024.0;

    frameLog.push_back(rec);

    if (frameLog.size() >= autoFlushFrameCount) {
        flushToDisk(csvFilename);
    }
}

void MetricsManager::flushToDisk(const std::string& filename) {
    if (frameLog.empty()) {
        mmLogInfo("[MetricsManager] flush requested but no buffered rows for " + filename);
        return;
    }

    bool needsHeader = !csvHeaderWritten;
    if (needsHeader) {
        std::ifstream check(filename);
        needsHeader = !check.good() || check.peek() == std::ifstream::traits_type::eof();
    }

    std::ofstream out(filename, std::ios::app);
    if (!out.is_open()) {
        mmLogError("[MetricsManager] Failed to open " + filename + " for CSV export");
        return;
    }

    if (needsHeader) {
        out << "Frame,TimeSec,FPS,AvgFPS,MinFPS,MaxFPS,CPUFrameTimeMs,GPUFrameTimeMs,CPUUsagePercent,MemoryMB\n";
        csvHeaderWritten = true;
    }

    size_t rowsWritten = frameLog.size();
    for (const auto& r : frameLog) {
        out << r.frameIndex << "," << r.timeSec << "," << r.fps << "," << r.avgFPS << ","
            << r.minFPS << "," << r.maxFPS << "," << r.cpuFrameTimeMs << "," << r.gpuFrameTimeMs
            << "," << r.cpuUsagePercent << "," << r.memoryMB << "\n";
    }

    out.close();
    frameLog.clear();
    mmLogInfo("[MetricsManager] Wrote " + std::to_string(rowsWritten) + " row(s) to " + filename);
}

void MetricsManager::exportCSV(const std::string& filename) {
    if (!filename.empty()) csvFilename = filename;
    flushToDisk(csvFilename);
}

#endif // platform split