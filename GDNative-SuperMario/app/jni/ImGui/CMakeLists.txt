cmake_minimum_required(VERSION 3.6)

add_library(imgui STATIC
        src/imgui.cpp
        src/imgui_draw.cpp
        src/imgui_widgets.cpp
        src/imgui_tables.cpp
        src/imgui_impl_sdl2.cpp
        src/imgui_impl_sdlrenderer2.cpp
)

target_include_directories(imgui PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../SDL/include
)