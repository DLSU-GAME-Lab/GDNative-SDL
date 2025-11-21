#include "PerformanceMetricsScreen.h"
#include "MetricsManager.h"

PerformanceMetricsScreen::PerformanceMetricsScreen() : AUIScreen("PERFORMANCE_METRICS_SCREEN")
{

}

PerformanceMetricsScreen::~PerformanceMetricsScreen()
{

}

void PerformanceMetricsScreen::DrawUI()
{

    //ImGui::Begin("Performance Metrics");

    //ImGui::Checkbox("Show FPS", &showFPS);
    //ImGui::Checkbox("Show CPU", &showCPU);
    //ImGui::Checkbox("Show Memory", &showMemory);
    //ImGui::Checkbox("Show GPU", &showGPU);
    //ImGui::Checkbox("Show Input Lag", &showInputLag);
    //ImGui::Checkbox("Show Load Time", &showLoadTime);
    //ImGui::Checkbox("Show Threads (app-only)", &showThreads);

    //if (ImGui::BeginTabBar("MetricsTabs"))
    //{
    //    if (showFPS && ImGui::BeginTabItem("FPS"))
    //    {
    //        ImGui::Text("FPS: %.1f", fps);
    //        ImGui::Text("Avg FPS: %.1f", avgFPS);
    //        ImGui::Text("Min FPS: %.1f", minFPS);
    //        ImGui::Text("Max FPS: %.1f", maxFPS);
    //        ImGui::Text("Frame time: %.2f ms", 1000.0f / (fps > 0.0f ? fps : 1.0f));
    //        ImGui::PlotLines("FPS History", fpsHistory, HISTORY_SIZE, offset, nullptr, 0.0f, 240.0f, ImVec2(0, 80));
    //        ImGui::EndTabItem();
    //    }

    //    if (showCPU && ImGui::BeginTabItem("CPU"))
    //    {
    //        ImGui::Text("CPU Usage (total): %.2f %%", cpuUsage);
    //        ImGui::Text("CPU Usage (per-core scaled): %.2f %%", cpuUsagePerCore);
    //        ImGui::PlotLines("CPU History", cpuHistory, HISTORY_SIZE, offset, nullptr, 0.0f, 100.0f, ImVec2(0, 80));

    //        if (pdhInitialized && logicalProcessorCount > 0)
    //        {
    //            int cols = 4;
    //            if (logicalProcessorCount < cols) cols = (int)logicalProcessorCount;
    //            ImGui::Columns(cols, "cores_cols", false);

    //            for (unsigned int i = 0; i < logicalProcessorCount; ++i)
    //            {
    //                char label[64];
    //                snprintf(label, sizeof(label), "Core %u: %.1f%%", i, corePercentages[i]);
    //                ImGui::Text("%s", label);
    //                float fraction = corePercentages[i] / 100.0f;
    //                if (fraction < 0.0f) fraction = 0.0f;
    //                if (fraction > 1.0f) fraction = 1.0f;
    //                ImGui::ProgressBar(fraction, ImVec2(-FLT_MIN, 0.0f), nullptr);
    //                if (i < coreHistory.size())
    //                {
    //                    std::string plotId = "##coreplot" + std::to_string(i);
    //                    ImGui::PlotLines(plotId.c_str(), coreHistory[i].data(), HISTORY_SIZE, offset, nullptr, 0.0f, 100.0f, ImVec2(0, 40));
    //                }
    //                ImGui::NextColumn();
    //            }
    //            ImGui::Columns(1);
    //        }
    //        else
    //        {
    //            ImGui::TextDisabled("Per-core counters not initialized.");
    //        }

    //        ImGui::EndTabItem();
    //    }

    //    if (showThreads && ImGui::BeginTabItem("Threads (app-only)"))
    //    {
    //        std::vector<ThreadStat> v;
    //        v.reserve(threadStats.size());
    //        for (auto& p : threadStats) v.push_back(p.second);
    //        std::sort(v.begin(), v.end(), [](const ThreadStat& a, const ThreadStat& b) { return a.usage > b.usage; });

    //        int showN = std::min((int)v.size(), 16);
    //        for (int i = 0; i < showN; ++i)
    //        {
    //            char title[64];
    //            snprintf(title, sizeof(title), "TID %u: %.1f%%", v[i].tid, v[i].usage);
    //            ImGui::Text("%s", title);
    //            float frac = v[i].usage / 100.0f;
    //            if (frac < 0.0f) frac = 0.0f;
    //            if (frac > 1.0f) frac = 1.0f;
    //            ImGui::ProgressBar(frac, ImVec2(-FLT_MIN, 0.0f), nullptr);

    //            std::string plotid = "##threadplot" + std::to_string(v[i].tid);
    //            ImGui::PlotLines(plotid.c_str(), v[i].history.data(), HISTORY_SIZE, offset, nullptr, 0.0f, 200.0f, ImVec2(0, 40));
    //            ImGui::Separator();
    //        }
    //        ImGui::EndTabItem();
    //    }

    //    if (showMemory && ImGui::BeginTabItem("Memory"))
    //    {
    //        ImGui::Text("Memory Usage: %.2f MB", memoryUsage / 1024.0 / 1024.0);
    //        ImGui::EndTabItem();
    //    }

    //    if (showGPU && ImGui::BeginTabItem("GPU"))
    //    {
    //        ImGui::Text("GPU Usage: %.2f %%", gpuUsage);
    //        ImGui::EndTabItem();
    //    }

    //    if (showInputLag && ImGui::BeginTabItem("Input Lag"))
    //    {
    //        ImGui::Text("Last Input Lag: %.2f ms", inputLagMs);
    //        ImGui::EndTabItem();
    //    }

    //    if (showLoadTime && ImGui::BeginTabItem("Load Time"))
    //    {
    //        ImGui::Text("Last Load Time: %.2f s", loadTimeSec);
    //        ImGui::EndTabItem();
    //    }

    //    ImGui::EndTabBar();
    //}

    //if (ImGui::Button("Export CSV"))
    //{
    //    MetricsManager::getInstance()->exportCSV();
    //}

    //ImGui::End();

}
