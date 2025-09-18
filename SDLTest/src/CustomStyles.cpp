#include "CustomStyles.h"

void Editor::CustomStyles::StyleColorsCrimson()
{
    ImGuiStyle* style = &ImGui::GetStyle();
    ImVec4* colors = style->Colors;

    ImVec4 clear = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    ImVec4 white = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    ImVec4 darkestRed = ImVec4(0.05f, 0.0f, 0.01f, 1.0f);
    ImVec4 darkRed = ImVec4(0.1f, 0.0f, 0.01f, 1.0f);
    ImVec4 midDarkRed = ImVec4(0.2f, 0.0f, 0.02f, 1.0f);
    ImVec4 midRed = ImVec4(0.5f, 0.05f, 0.12f, 1.0f);
    ImVec4 lightRed = ImVec4(0.8f, 0.1f, 0.2f, 1.0f);
    ImVec4 desatRed = ImVec4(0.8f, 0.25f, 0.3f, 1.0f);

    colors[ImGuiCol_Text] = white;
    colors[ImGuiCol_TextDisabled] = midRed;
    colors[ImGuiCol_WindowBg] = Alpha(darkRed, 0.95f);
    colors[ImGuiCol_ChildBg] = clear;
    colors[ImGuiCol_PopupBg] = darkRed;
    colors[ImGuiCol_Border] = Alpha(midRed, 0.95f);
    colors[ImGuiCol_BorderShadow] = clear;
    colors[ImGuiCol_FrameBg] = midDarkRed;
    colors[ImGuiCol_FrameBgHovered] = desatRed;
    colors[ImGuiCol_FrameBgActive] = lightRed;
    colors[ImGuiCol_TitleBg] = darkestRed;
    colors[ImGuiCol_TitleBgActive] = midDarkRed;
    colors[ImGuiCol_TitleBgCollapsed] = Alpha(darkestRed, 0.5f);
    colors[ImGuiCol_MenuBarBg] = darkestRed;
    colors[ImGuiCol_ScrollbarBg] = Alpha(darkestRed, 0.95f);
    colors[ImGuiCol_ScrollbarGrab] = Alpha(midDarkRed, 0.95f);
    colors[ImGuiCol_ScrollbarGrabHovered] = Alpha(desatRed, 0.95f);
    colors[ImGuiCol_ScrollbarGrabActive] = Alpha(lightRed, 0.95f);
    colors[ImGuiCol_CheckMark] = white;
    colors[ImGuiCol_SliderGrab] = Alpha(midRed, 0.95f);
    colors[ImGuiCol_SliderGrabActive] = Alpha(lightRed, 0.95f);
    colors[ImGuiCol_Button] = midRed;
    colors[ImGuiCol_ButtonHovered] = desatRed;
    colors[ImGuiCol_ButtonActive] = midDarkRed;
    colors[ImGuiCol_Header] = Alpha(midRed, 0.9f);
    colors[ImGuiCol_HeaderHovered] = Alpha(desatRed, 0.9f);
    colors[ImGuiCol_HeaderActive] = Alpha(lightRed, 0.9f);
    colors[ImGuiCol_Separator] = colors[ImGuiCol_Border];
    colors[ImGuiCol_SeparatorHovered] = desatRed;
    colors[ImGuiCol_SeparatorActive] = lightRed;
    colors[ImGuiCol_ResizeGrip] = Alpha(midRed, 0.5f);
    colors[ImGuiCol_ResizeGripHovered] = Alpha(midRed, 0.95f);
    colors[ImGuiCol_ResizeGripActive] = Alpha(lightRed, 0.5f);
    colors[ImGuiCol_TabHovered] = desatRed;
    colors[ImGuiCol_Tab] = midDarkRed;
    colors[ImGuiCol_TabSelected] = midRed;
    colors[ImGuiCol_TabSelectedOverline] = lightRed;
    colors[ImGuiCol_TabDimmed] = darkRed;
    colors[ImGuiCol_TabDimmedSelected] = midDarkRed;
    colors[ImGuiCol_TabDimmedSelectedOverline] = midRed;
    //colors[ImGuiCol_DockingPreview] = white;
    //colors[ImGuiCol_DockingEmptyBg] = darkRed;
    colors[ImGuiCol_PlotLines] = lightRed;
    colors[ImGuiCol_PlotLinesHovered] = desatRed;
    colors[ImGuiCol_PlotHistogram] = lightRed;
    colors[ImGuiCol_PlotHistogramHovered] = desatRed;
    colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
    colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
    colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
    colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
    colors[ImGuiCol_TextLink] = colors[ImGuiCol_HeaderActive];
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
    colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    colors[ImGuiCol_NavCursor] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
}

void Editor::CustomStyles::StyleColorsEvangelion()
{
    ImGuiStyle* style = &ImGui::GetStyle();
    ImVec4* colors = style->Colors;

    style->WindowBorderSize = 2.0f;
    style->ChildBorderSize = 1.0f;
    style->ChildRounding = 5.0f;
    style->TabBorderSize = 2.0f;
    style->TabBarBorderSize = 2.0f;
    style->TabRounding = 0.0f;

    ImVec4 clear = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    ImVec4 white = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    ImVec4 darkGray = ImVec4(0.08f, 0.05f, 0.1f, 1.0f);
    ImVec4 violet = ImVec4(0.25f, 0.1f, 0.4f, 1.0f);
    ImVec4 lavender = ImVec4(0.4f, 0.3f, 0.7f, 1.0f);
    ImVec4 green = ImVec4(0.0f, 0.9f, 0.1f, 1.0f);
    ImVec4 red = ImVec4(0.4f, 0.0f, 0.05f, 1.0f);
    ImVec4 orange = ImVec4(0.9f, 0.6f, 0.0f, 1.0f);

    colors[ImGuiCol_Text] = white;
    colors[ImGuiCol_TextDisabled] = lavender;
    colors[ImGuiCol_WindowBg] = Alpha(violet, 0.98f);
    colors[ImGuiCol_ChildBg] = Alpha(darkGray, 0.9f);
    colors[ImGuiCol_PopupBg] = violet;
    colors[ImGuiCol_Border] = Alpha(green, 0.98f);
    colors[ImGuiCol_BorderShadow] = clear;
    colors[ImGuiCol_FrameBg] = lavender;
    colors[ImGuiCol_FrameBgHovered] = red;
    colors[ImGuiCol_FrameBgActive] = orange;
    colors[ImGuiCol_TitleBg] = darkGray;
    colors[ImGuiCol_TitleBgActive] = green;
    colors[ImGuiCol_TitleBgCollapsed] = Alpha(darkGray, 0.8f);
    colors[ImGuiCol_MenuBarBg] = darkGray;
    colors[ImGuiCol_ScrollbarBg] = Alpha(darkGray, 0.9f);
    colors[ImGuiCol_ScrollbarGrab] = Alpha(orange, 0.9f);
    colors[ImGuiCol_ScrollbarGrabHovered] = Alpha(red, 0.9f);
    colors[ImGuiCol_ScrollbarGrabActive] = Alpha(lavender, 0.9f);
    colors[ImGuiCol_CheckMark] = white;
    colors[ImGuiCol_SliderGrab] = Alpha(orange, 0.9f);
    colors[ImGuiCol_SliderGrabActive] = Alpha(lavender, 0.9f);
    colors[ImGuiCol_Button] = green;
    colors[ImGuiCol_ButtonHovered] = red;
    colors[ImGuiCol_ButtonActive] = lavender;
    colors[ImGuiCol_Header] = Alpha(green, 0.9f);
    colors[ImGuiCol_HeaderHovered] = Alpha(red, 0.9f);
    colors[ImGuiCol_HeaderActive] = Alpha(lavender, 0.9f);
    colors[ImGuiCol_Separator] = colors[ImGuiCol_Border];
    colors[ImGuiCol_SeparatorHovered] = red;
    colors[ImGuiCol_SeparatorActive] = lavender;
    colors[ImGuiCol_ResizeGrip] = Alpha(orange, 0.95f);
    colors[ImGuiCol_ResizeGripHovered] = Alpha(red, 0.95f);
    colors[ImGuiCol_ResizeGripActive] = Alpha(lavender, 0.95f);
    colors[ImGuiCol_TabHovered] = red;
    colors[ImGuiCol_Tab] = darkGray;
    colors[ImGuiCol_TabSelected] = violet;
    colors[ImGuiCol_TabSelectedOverline] = clear;
    colors[ImGuiCol_TabDimmed] = darkGray;
    colors[ImGuiCol_TabDimmedSelected] = violet;
    colors[ImGuiCol_TabDimmedSelectedOverline] = clear;
    //colors[ImGuiCol_DockingPreview] = Alpha(orange, 0.9f);
    //colors[ImGuiCol_DockingEmptyBg] = violet;
    colors[ImGuiCol_PlotLines] = orange;
    colors[ImGuiCol_PlotLinesHovered] = red;
    colors[ImGuiCol_PlotHistogram] = orange;
    colors[ImGuiCol_PlotHistogramHovered] = red;
    colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
    colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
    colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
    colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
    colors[ImGuiCol_TextLink] = colors[ImGuiCol_HeaderActive];
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
    colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    colors[ImGuiCol_NavCursor] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
}

void Editor::CustomStyles::StyleColorsDarkForest()
{

}

ImVec4 Editor::CustomStyles::Alpha(const ImVec4& color, float a)
{
    return ImVec4(color.x, color.y, color.z, a);
}
