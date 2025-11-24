// MetricsManager.cpp
#include "MetricsManager.h"
#include "imgui.h"
#include <iostream>

#if !defined(_WIN32) && !defined(_WIN64)
#include <chrono>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <mutex>
#endif

// single definition for the singleton pointer (applies to both branches)
MetricsManager* MetricsManager::P_SHARED_INSTANCE = nullptr;

#if defined(_WIN32) || defined(_WIN64)

/* ---------------- Windows implementation (unchanged behaviour) ---------------- */

static inline unsigned long long FileTimeToULL(const FILETIME& ft) {
    return (((unsigned long long)ft.dwHighDateTime) << 32) | ft.dwLowDateTime;
}

/* ---------------- PDH helpers ---------------- */
bool MetricsManager::initPDH() {
    if (pdhInitialized) return true;
    PDH_STATUS st = PdhOpenQuery(NULL, 0, &hPdhQuery);
    if (st != ERROR_SUCCESS) {
        std::cerr << "[MetricsManager] PdhOpenQuery failed: " << st << std::endl;
        return false;
    }

    coreCounters.resize(logicalProcessorCount);
    corePercentages.assign(logicalProcessorCount, 0.0f);
    coreHistory.assign(logicalProcessorCount, std::vector<float>(HISTORY_SIZE, 0.0f));

    for (unsigned int i = 0; i < logicalProcessorCount; ++i) {
        std::string path = "\\Processor(" + std::to_string(i) + ")\\% Processor Time";
        PDH_STATUS addSt = PdhAddCounterA(hPdhQuery, path.c_str(), 0, &coreCounters[i]);
        if (addSt != ERROR_SUCCESS) {
            std::cerr << "[MetricsManager] PdhAddCounter failed for " << path << " (" << addSt << ")\n";
        }
    }

    PdhCollectQueryData(hPdhQuery);
    pdhInitialized = true;
    return true;
}

void MetricsManager::cleanupPDH() {
    if (!pdhInitialized) return;
    if (hPdhQuery) {
        PdhCloseQuery(hPdhQuery);
        hPdhQuery = NULL;
    }
    coreCounters.clear();
    corePercentages.clear();
    coreHistory.clear();
    pdhInitialized = false;
}

void MetricsManager::updatePDH() {
    if (!pdhInitialized) return;
    if (PdhCollectQueryData(hPdhQuery) != ERROR_SUCCESS) return;

    for (unsigned int i = 0; i < logicalProcessorCount; ++i) {
        PDH_FMT_COUNTERVALUE val;
        PDH_STATUS st = PdhGetFormattedCounterValue(coreCounters[i], PDH_FMT_DOUBLE, NULL, &val);
        double pct = 0.0;
        if (st == ERROR_SUCCESS) pct = val.doubleValue;
        corePercentages[i] = float((1.0 - cpuSmoothingAlpha) * (double)corePercentages[i] + cpuSmoothingAlpha * pct);
        if (i < coreHistory.size()) coreHistory[i][offset] = corePercentages[i];
    }
}

/* ---------------- Thread helpers ---------------- */
void MetricsManager::pruneDeadThreads(const std::vector<DWORD>& liveTids) {
    std::unordered_map<DWORD, ThreadStat> newMap;
    newMap.reserve(threadStats.size());
    for (DWORD tid : liveTids) {
        auto it = threadStats.find(tid);
        if (it != threadStats.end()) {
            newMap.emplace(tid, std::move(it->second));
        }
    }
    threadStats.swap(newMap);
}

void MetricsManager::updateThreadStats(double sampleIntervalSeconds) {
    if (sampleIntervalSeconds <= 0.0) return;
    unsigned long long interval100ns = (unsigned long long)(sampleIntervalSeconds * 1e7); // sec -> 100ns

    std::vector<DWORD> liveTids;
    liveTids.reserve(64);

    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    if (snap == INVALID_HANDLE_VALUE) return;

    THREADENTRY32 te;
    te.dwSize = sizeof(te);
    if (!Thread32First(snap, &te)) {
        CloseHandle(snap);
        return;
    }

    do {
        if (te.th32OwnerProcessID == GetCurrentProcessId()) {
            DWORD tid = te.th32ThreadID;
            liveTids.push_back(tid);

            HANDLE hThread = OpenThread(THREAD_QUERY_INFORMATION | THREAD_QUERY_LIMITED_INFORMATION, FALSE, tid);
            if (hThread == NULL) {
                hThread = OpenThread(THREAD_QUERY_INFORMATION, FALSE, tid);
            }
            if (hThread) {
                FILETIME createT, exitT, kernelT, userT;
                if (GetThreadTimes(hThread, &createT, &exitT, &kernelT, &userT)) {
                    unsigned long long total = FileTimeToULL(kernelT) + FileTimeToULL(userT);

                    auto it = threadStats.find(tid);
                    if (it == threadStats.end()) {
                        ThreadStat ts;
                        ts.tid = tid;
                        ts.prevTotal = total;
                        ts.usage = 0.0f;
                        ts.history.assign(HISTORY_SIZE, 0.0f);
                        threadStats.emplace(tid, std::move(ts));
                    }
                    else {
                        unsigned long long prev = it->second.prevTotal;
                        unsigned long long delta = (total >= prev) ? (total - prev) : 0ULL;
                        double pct = 0.0;
                        if (interval100ns > 0) pct = (double)delta * 100.0 / (double)interval100ns;

                        it->second.usage = float((1.0 - cpuSmoothingAlpha) * (double)it->second.usage + cpuSmoothingAlpha * pct);
                        it->second.prevTotal = total;
                        if (it->second.history.size() == 0) it->second.history.assign(HISTORY_SIZE, 0.0f);
                        it->second.history[offset] = it->second.usage;
                    }
                }
                CloseHandle(hThread);
            }
        }
    } while (Thread32Next(snap, &te));

    CloseHandle(snap);
    pruneDeadThreads(liveTids);
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
        QueryPerformanceCounter(&P_SHARED_INSTANCE->qpcLastTime);
        QueryPerformanceCounter(&P_SHARED_INSTANCE->qpcLastFrameTime);
        P_SHARED_INSTANCE->qpcLastThreadSampleTime = P_SHARED_INSTANCE->qpcLastFrameTime;

        // logical CPUs
        SYSTEM_INFO si;
        GetSystemInfo(&si);
        unsigned int count = si.dwNumberOfProcessors ? si.dwNumberOfProcessors : std::thread::hardware_concurrency();
        P_SHARED_INSTANCE->logicalProcessorCount = (count > 0) ? count : 1u;
        std::cout << "[MetricsManager] logical processors: " << P_SHARED_INSTANCE->logicalProcessorCount << std::endl;

        // PDH init (best-effort)
        P_SHARED_INSTANCE->initPDH();

        P_SHARED_INSTANCE->threadStatsInitialized = false;
    }
}

void MetricsManager::destroy() {
    if (P_SHARED_INSTANCE) {
        P_SHARED_INSTANCE->cleanupPDH();
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
    // timestamp
    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);

    // --- per-frame delta and FPS (instantaneous -> EMA-smoothed) ---
    double frameDelta = double(now.QuadPart - qpcLastFrameTime.QuadPart) / qpcFrequency;
    if (frameDelta <= 0.0) frameDelta = 1.0 / 1000.0; // defensive small delta
    double instFps = (frameDelta > 0.0) ? (1.0 / frameDelta) : 0.0;
    fps = float((1.0 - fpsSmoothingAlpha) * (double)fps + fpsSmoothingAlpha * instFps);

    // one-second bucket for min/max and optional bucketed stats
    frameCount++;
    double sinceFpsBucket = double(now.QuadPart - qpcLastTime.QuadPart) / qpcFrequency;
    if (sinceFpsBucket >= 1.0) {
        if (fps < minFPS) minFPS = fps;
        if (fps > maxFPS) maxFPS = fps;
        qpcLastTime = now;
    }

    // --- Process-wide CPU via FILETIME ---
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

            double cpuPctPerCore = cpuPctTotal * (double)logicalProcessorCount;

            cpuUsage = (1.0 - cpuSmoothingAlpha) * cpuUsage + cpuSmoothingAlpha * cpuPctTotal;
            cpuUsagePerCore = (1.0 - cpuSmoothingAlpha) * cpuUsagePerCore + cpuSmoothingAlpha * cpuPctPerCore;
        }
        else {
            cpuUsage = 0.0;
            cpuUsagePerCore = 0.0;
        }

        prevSysIdle = sysIdle;
        prevSysKernel = sysKernel;
        prevSysUser = sysUser;
        prevProcKernel = procKernel;
        prevProcUser = procUser;
        cpuInitialized = true;
    }

    // --- PDH per-core (system-wide) ---
    updatePDH();

    // --- Per-thread (app-only) update (throttled) ---
    double sinceThreadSample = double(now.QuadPart - qpcLastThreadSampleTime.QuadPart) / qpcFrequency;
    if (sinceThreadSample >= threadSampleIntervalSec) {
        updateThreadStats(sinceThreadSample);
        qpcLastThreadSampleTime = now;
    }

    // --- Memory ---
    PROCESS_MEMORY_COUNTERS_EX pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc))) {
        memoryUsage = pmc.WorkingSetSize;
    }

    // --- Unified history write (FPS updated every frame) ---
    fpsHistory[offset] = fps;
    cpuHistory[offset] = (float)cpuUsage;

    offset = (offset + 1) % HISTORY_SIZE;
    if (historyCount < HISTORY_SIZE) ++historyCount;

    // recompute avgFPS from actual history contents
    float sum = 0.0f;
    for (int i = 0; i < historyCount; ++i) sum += fpsHistory[i];
    avgFPS = (historyCount > 0) ? (sum / historyCount) : fps;

    // prepare for next frame
    qpcLastFrameTime = now;
}

/* ---------------- Logging & GUI ---------------- */
void MetricsManager::logMetrics() {
    std::cout << "[Metrics] FPS=" << fps
        << " | AvgFPS=" << avgFPS
        << " | MinFPS=" << minFPS
        << " | MaxFPS=" << maxFPS
        << " | CPU(total%)=" << cpuUsage
        << " | CPU(per-core%)=" << cpuUsagePerCore
        << " | Mem=" << (memoryUsage / 1024.0 / 1024.0) << " MB"
        << std::endl;

    std::vector<ThreadStat> v;
    v.reserve(threadStats.size());
    for (auto& p : threadStats) v.push_back(p.second);
    std::sort(v.begin(), v.end(), [](const ThreadStat& a, const ThreadStat& b) { return a.usage > b.usage; });
    int toShow = std::min((size_t)5, v.size());
    for (int i = 0; i < toShow; ++i) {
        std::cout << "  [Thread] TID=" << v[i].tid << " CPU=" << v[i].usage << "%\n";
    }
}

void MetricsManager::drawGUI() {
    ImGui::Begin("Performance Metrics");

    ImGui::Checkbox("Show FPS", &showFPS);
    ImGui::Checkbox("Show CPU", &showCPU);
    ImGui::Checkbox("Show Memory", &showMemory);
    ImGui::Checkbox("Show GPU", &showGPU);
    ImGui::Checkbox("Show Input Lag", &showInputLag);
    ImGui::Checkbox("Show Load Time", &showLoadTime);
    ImGui::Checkbox("Show Threads (app-only)", &showThreads);

    if (ImGui::BeginTabBar("MetricsTabs")) {
        if (showFPS && ImGui::BeginTabItem("FPS")) {
            ImGui::Text("FPS: %.1f", fps);
            ImGui::Text("Avg FPS: %.1f", avgFPS);
            ImGui::Text("Min FPS: %.1f", minFPS);
            ImGui::Text("Max FPS: %.1f", maxFPS);
            ImGui::Text("Frame time: %.2f ms", 1000.0f / (fps > 0.0f ? fps : 1.0f));
            ImGui::PlotLines("FPS History", fpsHistory, HISTORY_SIZE, offset, nullptr, 0.0f, 240.0f, ImVec2(0, 80));
            ImGui::EndTabItem();
        }

        if (showCPU && ImGui::BeginTabItem("CPU")) {
            ImGui::Text("CPU Usage (total): %.2f %%", cpuUsage);
            ImGui::Text("CPU Usage (per-core scaled): %.2f %%", cpuUsagePerCore);
            ImGui::PlotLines("CPU History", cpuHistory, HISTORY_SIZE, offset, nullptr, 0.0f, 100.0f, ImVec2(0, 80));

            if (pdhInitialized && logicalProcessorCount > 0) {
                int cols = 4;
                if (logicalProcessorCount < cols) cols = (int)logicalProcessorCount;
                ImGui::Columns(cols, "cores_cols", false);

                for (unsigned int i = 0; i < logicalProcessorCount; ++i) {
                    char label[64];
                    snprintf(label, sizeof(label), "Core %u: %.1f%%", i, corePercentages[i]);
                    ImGui::Text("%s", label);
                    float fraction = corePercentages[i] / 100.0f;
                    if (fraction < 0.0f) fraction = 0.0f;
                    if (fraction > 1.0f) fraction = 1.0f;
                    ImGui::ProgressBar(fraction, ImVec2(-FLT_MIN, 0.0f), nullptr);
                    if (i < coreHistory.size()) {
                        std::string plotId = "##coreplot" + std::to_string(i);
                        ImGui::PlotLines(plotId.c_str(), coreHistory[i].data(), HISTORY_SIZE, offset, nullptr, 0.0f, 100.0f, ImVec2(0, 40));
                    }
                    ImGui::NextColumn();
                }
                ImGui::Columns(1);
            }
            else {
                ImGui::TextDisabled("Per-core counters not initialized.");
            }

            ImGui::EndTabItem();
        }

        if (showThreads && ImGui::BeginTabItem("Threads (app-only)")) {
            std::vector<ThreadStat> v;
            v.reserve(threadStats.size());
            for (auto& p : threadStats) v.push_back(p.second);
            std::sort(v.begin(), v.end(), [](const ThreadStat& a, const ThreadStat& b) { return a.usage > b.usage; });

            int showN = std::min((int)v.size(), 16);
            for (int i = 0; i < showN; ++i) {
                char title[64];
                snprintf(title, sizeof(title), "TID %u: %.1f%%", v[i].tid, v[i].usage);
                ImGui::Text("%s", title);
                float frac = v[i].usage / 100.0f;
                if (frac < 0.0f) frac = 0.0f;
                if (frac > 1.0f) frac = 1.0f;
                ImGui::ProgressBar(frac, ImVec2(-FLT_MIN, 0.0f), nullptr);

                std::string plotid = "##threadplot" + std::to_string(v[i].tid);
                ImGui::PlotLines(plotid.c_str(), v[i].history.data(), HISTORY_SIZE, offset, nullptr, 0.0f, 200.0f, ImVec2(0, 40));
                ImGui::Separator();
            }
            ImGui::EndTabItem();
        }

        if (showMemory && ImGui::BeginTabItem("Memory")) {
            ImGui::Text("Memory Usage: %.2f MB", memoryUsage / 1024.0 / 1024.0);
            ImGui::EndTabItem();
        }

        if (showGPU && ImGui::BeginTabItem("GPU")) {
            ImGui::Text("GPU Usage: %.2f %%", gpuUsage);
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

/* ---------------- CSV / helpers ---------------- */
void MetricsManager::exportCSV(const std::string& filename) {
    std::ofstream out(filename, std::ios::app);
    if (out.is_open()) {
        out << fps << "," << avgFPS << "," << minFPS << "," << maxFPS << ","
            << cpuUsage << "," << (memoryUsage / 1024.0 / 1024.0) <<
            "," << gpuUsage << "," << inputLagMs << "," << loadTimeSec << std::endl;
        out.close();
        std::cout << "[MetricsManager] Exported metrics to " << filename << std::endl;
    }
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

#else // non-Windows fallback implementation (Android)

using namespace std::chrono;

MetricsManager::MetricsManager() {
    lastFrameTime = steady_clock::now();
    lastSampleTime = lastFrameTime;
    offset = 0;
    historyCount = 0;
}

MetricsManager::~MetricsManager() {}

void MetricsManager::initialize() {
    if (!P_SHARED_INSTANCE) {
        P_SHARED_INSTANCE = new MetricsManager();
    }
}

void MetricsManager::destroy() {
    if (P_SHARED_INSTANCE) {
        delete P_SHARED_INSTANCE;
        P_SHARED_INSTANCE = nullptr;
    }
}

MetricsManager* MetricsManager::getInstance() {
    if (!P_SHARED_INSTANCE) initialize();
    return P_SHARED_INSTANCE;
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
    inputLagMs = (nowMs > inputTimestamp) ? (nowMs - inputTimestamp) : 0;
    inputTimestamp = 0;
}

void MetricsManager::update() {
    auto now = steady_clock::now();
    double dt = duration<double>(now - lastFrameTime).count();
    lastFrameTime = now;

    if (dt <= 0.0) dt = 1.0/1000.0;

    ++frameCount;
    float instFPS = static_cast<float>(1.0 / dt);
    fps = static_cast<float>((fps * (1.0 - fpsSmoothingAlpha)) + (instFPS * fpsSmoothingAlpha));

    // history
    fpsHistory[offset] = fps;
    cpuHistory[offset] = static_cast<float>(cpuUsage);
    offset = (offset + 1) % HISTORY_SIZE;
    if (historyCount < HISTORY_SIZE) ++historyCount;

    // recompute avgFPS
    float sum = 0.0f;
    for (int i = 0; i < historyCount; ++i) sum += fpsHistory[i];
    avgFPS = (historyCount > 0) ? (sum / historyCount) : fps;
}

void MetricsManager::logMetrics() {
    std::cout << "[Metrics] FPS: " << std::fixed << std::setprecision(2) << fps
              << " avg: " << avgFPS
              << " min: " << minFPS
              << " max: " << maxFPS
              << " cpu: " << cpuUsage
              << " mem: " << memoryUsage
              << "\n";
}

void MetricsManager::drawGUI() {
    ImGui::Begin("Performance Metrics");

    ImGui::Checkbox("Show FPS", &showFPS);
    ImGui::Checkbox("Show CPU", &showCPU);
    ImGui::Checkbox("Show Memory", &showMemory);
    ImGui::Checkbox("Show GPU", &showGPU);
    ImGui::Checkbox("Show Input Lag", &showInputLag);
    ImGui::Checkbox("Show Load Time", &showLoadTime);
    ImGui::Checkbox("Show Threads (app-only)", &showThreads);

    if (ImGui::BeginTabBar("MetricsTabs")) {
        if (showFPS && ImGui::BeginTabItem("FPS")) {
            ImGui::Text("FPS: %.1f", fps);
            ImGui::Text("Avg FPS: %.1f", avgFPS);
            ImGui::Text("Min FPS: %.1f", minFPS);
            ImGui::Text("Max FPS: %.1f", maxFPS);
            ImGui::Text("Frame time: %.2f ms", 1000.0f / (fps > 0.0f ? fps : 1.0f));
            ImGui::PlotLines("FPS History", fpsHistory, HISTORY_SIZE, offset, nullptr, 0.0f, 240.0f, ImVec2(0, 80));
            ImGui::EndTabItem();
        }

        if (showCPU && ImGui::BeginTabItem("CPU")) {
            ImGui::Text("CPU Usage (total): %.2f %%", cpuUsage);
            ImGui::PlotLines("CPU History", cpuHistory, HISTORY_SIZE, offset, nullptr, 0.0f, 100.0f, ImVec2(0, 80));
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

void MetricsManager::exportCSV(const std::string& filename) {
    std::ofstream out(filename, std::ios::app);
    if (out.is_open()) {
        out << fps << "," << avgFPS << "," << minFPS << "," << maxFPS << ","
            << cpuUsage << "," << (memoryUsage / 1024.0 / 1024.0) <<
            "," << gpuUsage << "," << inputLagMs << "," << loadTimeSec << std::endl;
        out.close();
    }
}

#endif // platform split