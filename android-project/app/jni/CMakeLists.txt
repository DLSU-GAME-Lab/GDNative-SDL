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
	${HEADERS}
)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
        sdl3_wrapper
	sdl3_image_wrapper
	sdl3_ttf_wrapper
	imgui_headers
        spdlog_headers
        ${android}
        ${log}
	${GLESv3-lib}
)