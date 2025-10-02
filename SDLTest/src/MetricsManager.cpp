#include "MetricsManager.h"
#include "imgui.h"

MetricsManager* MetricsManager::P_SHARED_INSTANCE = nullptr;

void MetricsManager::initialize() {
    if (!P_SHARED_INSTANCE) {
        P_SHARED_INSTANCE = new MetricsManager();
        std::cout << "[MetricsManager] Initialized." << std::endl;

    #ifdef _WIN32
        LARGE_INTEGER freq;
        QueryPerformanceFrequency(&freq);
        P_SHARED_INSTANCE->qpcFrequency = double(freq.QuadPart);
        QueryPerformanceCounter(&P_SHARED_INSTANCE->qpcLastTime);
    #elif defined(__ANDROID__)
        P_SHARED_INSTANCE->lastTime = SDL_GetPerformanceCounter();
    #endif
    }
}

void MetricsManager::destroy() {
    if (P_SHARED_INSTANCE) {
        delete P_SHARED_INSTANCE;
        P_SHARED_INSTANCE = nullptr;
        std::cout << "[MetricsManager] Destroyed." << std::endl;
    }
}

MetricsManager* MetricsManager::getInstance() {
    return P_SHARED_INSTANCE;
}

void MetricsManager::update() {
    frameCount++;

    // --- FPS ---
#ifdef _WIN32
    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);
    double elapsed = double(now.QuadPart - qpcLastTime.QuadPart) / qpcFrequency;
    if (elapsed >= 1.0) {
        fps = float(frameCount / elapsed);
        frameCount = 0;
        qpcLastTime = now;
    }
#elif defined(__ANDROID__)
    Uint64 now = SDL_GetPerformanceCounter();
    double elapsed = (double)(now - lastTime) / SDL_GetPerformanceFrequency();
    if (elapsed >= 1.0) {
        fps = float(frameCount / elapsed);
        frameCount = 0;
        lastTime = now;
    }
#endif

    // --- CPU ---
#ifdef _WIN32
    FILETIME sysIdle, sysKernel, sysUser;
    FILETIME procCreation, procExit, procKernel, procUser;

    if (GetSystemTimes(&sysIdle, &sysKernel, &sysUser) &&
        GetProcessTimes(GetCurrentProcess(), &procCreation, &procExit, &procKernel, &procUser)) {
        if (cpuInitialized) {
            ULONGLONG sysKernelDiff = (((ULONGLONG)sysKernel.dwHighDateTime << 32) | sysKernel.dwLowDateTime) -
                (((ULONGLONG)prevSysKernel.dwHighDateTime << 32) | prevSysKernel.dwLowDateTime);
            ULONGLONG sysUserDiff = (((ULONGLONG)sysUser.dwHighDateTime << 32) | sysUser.dwLowDateTime) -
                (((ULONGLONG)prevSysUser.dwHighDateTime << 32) | prevSysUser.dwLowDateTime);

            ULONGLONG procKernelDiff = (((ULONGLONG)procKernel.dwHighDateTime << 32) | procKernel.dwLowDateTime) -
                (((ULONGLONG)prevProcKernel.dwHighDateTime << 32) | prevProcKernel.dwLowDateTime);
            ULONGLONG procUserDiff = (((ULONGLONG)procUser.dwHighDateTime << 32) | procUser.dwLowDateTime) -
                (((ULONGLONG)prevProcUser.dwHighDateTime << 32) | prevProcUser.dwLowDateTime);

            ULONGLONG sysTotal = sysKernelDiff + sysUserDiff;
            ULONGLONG procTotal = procKernelDiff + procUserDiff;

            if (sysTotal > 0) {
                cpuUsage = (double)(procTotal * 100.0 / sysTotal);
            }
        }

        prevSysKernel = sysKernel;
        prevSysUser = sysUser;
        prevProcKernel = procKernel;
        prevProcUser = procUser;
        cpuInitialized = true;
    }
#elif defined(__ANDROID__)
    struct rusage usage;
    if (getrusage(RUSAGE_SELF, &usage) == 0) {
        Uint64 currCPUTime = usage.ru_utime.tv_sec * 1000000ULL + usage.ru_utime.tv_usec;
        if (cpuInitialized) {
            Uint64 diff = currCPUTime - lastCPUTime;
            cpuUsage = (double)diff / 10000.0; // rough % estimate
        }
        lastCPUTime = currCPUTime;
        cpuInitialized = true;
    }
#endif

    // --- Memory ---
#ifdef _WIN32
    PROCESS_MEMORY_COUNTERS_EX pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc))) {
        memoryUsage = pmc.WorkingSetSize; // bytes
    }
#elif defined(__ANDROID__)
    struct rusage mem;
    if (getrusage(RUSAGE_SELF, &mem) == 0) {
        memoryUsage = mem.ru_maxrss * 1024; // ru_maxrss is KB
    }
#endif

    // --- Update history buffers ---
    fpsHistory[offset] = fps;
    cpuHistory[offset] = (float)cpuUsage;
    offset = (offset + 1) % HISTORY_SIZE;
}

void MetricsManager::logMetrics() {
    std::cout << "[Metrics] FPS=" << fps
        << " | CPU=" << cpuUsage << "%"
        << " | Mem=" << (memoryUsage / 1024.0 / 1024.0) << " MB"
        << std::endl;
}

void MetricsManager::drawGUI() {
    ImGui::Begin("Performance Metrics");

    ImGui::Checkbox("Show FPS", &showFPS);
    ImGui::Checkbox("Show CPU", &showCPU);
    ImGui::Checkbox("Show Memory", &showMemory);

    if (ImGui::BeginTabBar("MetricsTabs")) {
        if (showFPS && ImGui::BeginTabItem("FPS")) {
            ImGui::Text("FPS: %.1f", fps);
            ImGui::PlotLines("FPS History", fpsHistory, HISTORY_SIZE,
                offset, nullptr, 0.0f, 120.0f, ImVec2(0, 80));
            ImGui::EndTabItem();
        }

        if (showCPU && ImGui::BeginTabItem("CPU")) {
            ImGui::Text("CPU Usage: %.2f %%", cpuUsage);
            ImGui::PlotLines("CPU History", cpuHistory, HISTORY_SIZE,
                offset, nullptr, 0.0f, 100.0f, ImVec2(0, 80));
            ImGui::EndTabItem();
        }

        if (showMemory && ImGui::BeginTabItem("Memory")) {
            ImGui::Text("Memory Usage: %.2f MB", memoryUsage / 1024.0 / 1024.0);
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    if (ImGui::Button("Export CSV")) {
        exportCSV();
    }

    ImGui::End();
}

void MetricsManager::exportCSV(const std::string& filename) {
    std::ofstream out(filename, std::ios::app);
    if (out.is_open()) {
        out << fps << "," << cpuUsage << "," << (memoryUsage / 1024.0 / 1024.0) << std::endl;
        out.close();
        std::cout << "[MetricsManager] Exported metrics to " << filename << std::endl;
    }
}
