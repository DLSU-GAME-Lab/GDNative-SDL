cmake_minimum_required(VERSION 3.22.1)

project("Babaylan_Tales")

find_library(log log)
find_library(android android)

file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")

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
        spdlog_headers
        ${android}
        ${log}
)