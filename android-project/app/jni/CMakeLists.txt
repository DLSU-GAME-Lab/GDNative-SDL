cmake_minimum_required(VERSION 3.22.1)

project("Babaylan_Tales")

find_library(log log)
find_library(android android)
find_library(GLESv3-lib GLESv3)

file(GLOB SOURCES 
	"${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
        "${imgui_SOURCE_DIR}/imgui.cpp"
    	"${imgui_SOURCE_DIR}/imgui_draw.cpp"
    	"${imgui_SOURCE_DIR}/imgui_tables.cpp"
    	"${imgui_SOURCE_DIR}/imgui_widgets.cpp"
    	"${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp"
    	"${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp"
	"${imgui_SOURCE_DIR}/backends/imgui_impl_sdlrenderer3.cpp"
)

add_library(${CMAKE_PROJECT_NAME} SHARED
        ${SOURCES}
)

# Expose your local include dir
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Make sure we include ImGui headers and SDL include directories produced by top-level
# The following variables (SDL3_INSTALL_DIR etc.) are defined in the top-level CMake
# and are visible here because add_subdirectory(jni) is used.
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
    ${SDL3_INSTALL_DIR}/include
    ${SDL3_IMAGE_INSTALL_DIR}/include
    ${SDL_TTF_INSTALL_DIR}/include
)

# Link against the imported SDL (and image/ttf) targets directly.
# Also link against ImGui headers (interface), spdlog headers (interface), and platform libs.
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
    SDL3
    SDL3_image
    SDL3_ttf
    imgui_headers
    spdlog_headers
    ${android}
    ${log}
    ${GLESv3-lib}
)

# Ensure the external_prebuild target (which builds SDL and copies .so files) runs before your native library builds.
# NOTE: external_prebuild is defined in the top-level CMake (see previous instructions).
add_dependencies(${CMAKE_PROJECT_NAME} external_prebuild)