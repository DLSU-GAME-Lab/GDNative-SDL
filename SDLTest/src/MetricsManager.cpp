#include "MetricsManager.h"
#include "imgui.h"

MetricsManager* MetricsManager::P_SHARED_INSTANCE = nullptr;

void MetricsManager::initialize() {
    if (!P_SHARED_INSTANCE) {
        P_SHARED_INSTANCE = new MetricsManager();
        std::cout << "[MetricsManager] Initialized." << std::endl;

        // Init high-res timer
        LARGE_INTEGER freq;
        QueryPerformanceFrequency(&freq);
        P_SHARED_INSTANCE->qpcFrequency = double(freq.QuadPart);
        QueryPerformanceCounter(&P_SHARED_INSTANCE->qpcLastTime);
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
    // --- FPS (high precision with QPC) ---
    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);
    frameCount++;
    double elapsed = double(now.QuadPart - qpcLastTime.QuadPart) / qpcFrequency;
    if (elapsed >= 1.0) {
        fps = float(frameCount / elapsed);
        frameCount = 0;
        qpcLastTime = now;
    }

    // --- CPU ---
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

    // --- Memory ---
    PROCESS_MEMORY_COUNTERS_EX pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc))) {
        memoryUsage = pmc.WorkingSetSize; // bytes
    }

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
    ImGui::Checkbox("Show GPU", &showGPU);
    ImGui::Checkbox("Show Input Lag", &showInputLag); 
    ImGui::Checkbox("Show Load Time", &showLoadTime);

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

void MetricsManager::exportCSV(const std::string& filename) {
    std::ofstream out(filename, std::ios::app);
    if (out.is_open()) {
        out << fps << "," << cpuUsage << "," << (memoryUsage / 1024.0 / 1024.0) << 
            "," << gpuUsage << "," << inputLagMs << "," << loadTimeSec << std::endl;
        out.close();
        std::cout << "[MetricsManager] Exported metrics to " << filename << std::endl;
    }
}

// --- load time helpers ---
void MetricsManager::startLoadTimer() {
    QueryPerformanceCounter(&loadStart);
}

void MetricsManager::endLoadTimer() {
    LARGE_INTEGER end;
    QueryPerformanceCounter(&end);
    loadTimeSec = double(end.QuadPart - loadStart.QuadPart) / qpcFrequency;
}

// --- input lag helpers ---
// call recordInputEvent() when key or mouse button is pressed

void MetricsManager::recordInputEvent() {
    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);
    inputTimestamp = now.QuadPart;
}

// call markInputHandled() when the frame reacts visually to input
void MetricsManager::markInputHandled() {
    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);
    if (inputTimestamp != 0) {
        double diff = double(now.QuadPart - inputTimestamp) / qpcFrequency; 
        inputLagMs = diff * 1000.0; 
        inputTimestamp = 0; // reset 
    } 
}