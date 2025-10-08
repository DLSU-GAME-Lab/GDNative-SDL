cmake_minimum_required(VERSION 3.22.1)

include(FetchContent)
include(ExternalProject)

project("Babaylan_Tales")

# Version Management (Optional)
set(SAMPLE_VERSION_MAJOR 1)
set(SAMPLE_VERSION_MINOR 0)
set(SAMPLE_VERSION_PATCH 0)
set(SAMPLE_VERSION "${SAMPLE_VERSION_MAJOR}.${SAMPLE_VERSION_MINOR}.${SAMPLE_VERSION_PATCH}")

# Configurations (just copy paste these)
# ==========================================================

option(SAMPLEAPP_BUILD_TESTS "Build tests" OFF)
option(SAMPLEAPP_USE_BUNDLED_DEPS "Use bundled dependencies" ON)

set(CMAKE_DISABLE_IN_SOURCE_BUILDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Android-specific settings
if(ANDROID)
    # For ninja.exe
    # Derive SDK path from NDK path (Gradle always defines ANDROID_NDK)
    get_filename_component(ANDROID_SDK "${ANDROID_NDK}/.." ABSOLUTE)

    # Point CMake to Ninja inside the SDK's cmake folder
    find_program(NINJA_EXECUTABLE ninja
        HINTS "${ANDROID_SDK}/cmake/3.22.1/bin"
    )

    if(NINJA_EXECUTABLE)
        set(CMAKE_MAKE_PROGRAM "${NINJA_EXECUTABLE}"
            CACHE FILEPATH "Ninja executable" FORCE)
    else()
        message(FATAL_ERROR "Ninja not found in ${ANDROID_SDK}/cmake/3.22.1/bin. Please install via SDK Manager.")
    endif()

    # Android-specific settings
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DANDROID -D__ANDROID__")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DANDROID -D__ANDROID__ -D__ANDROID_USE_LIBCXX=1")

    # Prevent Windows library linking attempts
    set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
    set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
    set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
endif()

# Android-APK settings
if(ANDROID)
    if(NOT ANDROID_ABI)
        message(FATAL_ERROR "ANDROID_ABI not set! Make sure Gradle passes -DANDROID_ABI")
    endif()
    if(NOT ANDROID_PLATFORM)
        set(ANDROID_PLATFORM android-21)
    endif()

    set(CMAKE_TOOLCHAIN_FILE "${ANDROID_NDK}/build/cmake/android.toolchain.cmake")
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

# =============================================================

# Change to "Release" if Release lmao
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Use a single merged-jni path variable for copying .so files
set(MERGED_JNI_OUT "${CMAKE_CURRENT_SOURCE_DIR}/app/build/intermediates/merged_jni_libs/${CMAKE_BUILD_TYPE}/out/${ANDROID_ABI}")
file(MAKE_DIRECTORY ${MERGED_JNI_OUT})

# ================= SDL Directories Creation ==================
set(SDL3_INSTALL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/build/SDL3")
file(MAKE_DIRECTORY ${SDL3_INSTALL_DIR}/lib)
file(MAKE_DIRECTORY ${SDL3_INSTALL_DIR}/include)

set(SDL3_IMAGE_INSTALL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/build/SDL3_image")
file(MAKE_DIRECTORY ${SDL3_IMAGE_INSTALL_DIR}/lib)
file(MAKE_DIRECTORY ${SDL3_IMAGE_INSTALL_DIR}/include)

set(SDL_TTF_INSTALL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/build/SDL_ttf")
file(MAKE_DIRECTORY ${SDL_TTF_INSTALL_DIR}/lib)
file(MAKE_DIRECTORY ${SDL_TTF_INSTALL_DIR}/include)

# ================= Add SDL3 via ExternalProject =============================
ExternalProject_Add(external_sdl3
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG release-3.2.22
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
        PREFIX ${CMAKE_BINARY_DIR}/external/SDL3
        CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${SDL3_INSTALL_DIR}
        -DCMAKE_POSITION_INDEPENDENT_CODE=ON
        -DANDROID_STL=c++_shared
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DCMAKE_MAKE_PROGRAM=${CMAKE_MAKE_PROGRAM}
        -DSDL_INSTALL=ON
        -DSDL_SHARED=ON
        -DSDL_STATIC=OFF
        -DSDL_ANDROID=ON
        -DSDL_VULKAN=ON
        -DSDL_HAPTIC=OFF
        -DSDL_POWER=OFF
        -DSDL_OPENGL=ON
        -DSDL_OPENGLES=ON
        -DSDL_METAL=OFF
        -DSDL_JOYSTICK=ON
        -DSDL_RENDER=ON
        -DSDL_AUDIO=ON
        -DSDL_VIDEO=ON
        -DSDL_EVENTS=ON
        -DSDL_ATOMIC=ON
        -DSDL_THREADS=ON
        -DSDL_TIMER=ON
        -DSDL_HIDAPI=ON
        -DSDL_DEPS_SHARED=OFF
        -DSDL_EXAMPLES=OFF
        -DSDL_TESTS=OFF
        -DSDL_DUMMYAUDIO=OFF
        -DSDL_DISKAUDIO=OFF
        -DCMAKE_C_FLAGS=-DSDL_HIDAPI_DISABLED=1
        -DCMAKE_CXX_FLAGS=-DSDL_HIDAPI_DISABLED=1
        UPDATE_COMMAND ""
        BUILD_BYPRODUCTS ${SDL3_INSTALL_DIR}/lib/libSDL3.so
        CMAKE_CACHE_ARGS
        -DCMAKE_TOOLCHAIN_FILE:PATH=${CMAKE_TOOLCHAIN_FILE}
        -DANDROID_PLATFORM:STRING=${ANDROID_PLATFORM}
        -DANDROID_ABI:STRING=${ANDROID_ABI}
)
# ====================================================================================

# Import SDL3 as an IMPORTED library (will be produced by ExternalProject)
add_library(SDL3 SHARED IMPORTED GLOBAL)
set_target_properties(SDL3 PROPERTIES
        IMPORTED_LOCATION "${SDL3_INSTALL_DIR}/lib/libSDL3.so"
        INTERFACE_INCLUDE_DIRECTORIES "${SDL3_INSTALL_DIR}/include"
)
target_compile_definitions(SDL3 INTERFACE
        ANDROID
        __ANDROID__
)
# ensure association so dependency graph knows external_sdl3 produces it
add_dependencies(SDL3 external_sdl3)

# ================= SDL3_image ExternalProject ==================
ExternalProject_Add(external_sdl3_image
        GIT_REPOSITORY  https://github.com/libsdl-org/SDL_image.git
        GIT_TAG release-3.2.2
        GIT_SHALLOW FALSE
        GIT_PROGRESS TRUE
        PREFIX ${CMAKE_BINARY_DIR}/external/SDL3_image
        CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${SDL3_IMAGE_INSTALL_DIR}
        -DCMAKE_POSITION_INDEPENDENT_CODE=ON
        -DANDROID_STL=c++_shared
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DCMAKE_MAKE_PROGRAM=${CMAKE_MAKE_PROGRAM}
        -DSDL3IMAGE_INSTALL=ON
        -DSDL3IMAGE_DEPS_SHARED=OFF
        -DSDL3IMAGE_VENDORED=ON
        -DSDL3IMAGE_BUILD_SHARED_LIBS=ON
        -DSDL3IMAGE_JPG_SHARED=OFF
        -DSDL3IMAGE_PNG_SHARED=OFF
        -DSDL3IMAGE_TIF_SHARED=OFF
        -DSDL3IMAGE_WEBP_SHARED=OFF
        -DSDL3IMAGE_JPG=ON
        -DSDL3IMAGE_PNG=ON
        -DSDL3IMAGE_SAMPLES=OFF
        -DSDL3IMAGE_TESTS=OFF
        -DSDL3_DIR=${SDL3_INSTALL_DIR}/lib/cmake/SDL3
        -DCMAKE_PREFIX_PATH=${SDL3_INSTALL_DIR}
        UPDATE_COMMAND ""
        BUILD_BYPRODUCTS ${SDL3_IMAGE_INSTALL_DIR}/lib/libSDL3_image.so
        DEPENDS external_sdl3
        CMAKE_CACHE_ARGS
        -DCMAKE_TOOLCHAIN_FILE:PATH=${CMAKE_TOOLCHAIN_FILE}
        -DANDROID_PLATFORM:STRING=${ANDROID_PLATFORM}
        -DANDROID_ABI:STRING=${ANDROID_ABI}
)

# Import SDL3_image as IMPORTED
add_library(SDL3_image SHARED IMPORTED GLOBAL)
set_target_properties(SDL3_image PROPERTIES
    IMPORTED_LOCATION "${SDL3_IMAGE_INSTALL_DIR}/lib/libSDL3_image.so"
    INTERFACE_INCLUDE_DIRECTORIES "${SDL3_IMAGE_INSTALL_DIR}/include"
)
add_dependencies(SDL3_image external_sdl3_image)

# Provide an interface wrapper
add_library(sdl3_image_wrapper INTERFACE)
target_include_directories(sdl3_image_wrapper INTERFACE ${SDL3_IMAGE_INSTALL_DIR}/include)
target_link_libraries(sdl3_image_wrapper INTERFACE SDL3 SDL3_image)
add_dependencies(sdl3_image_wrapper external_sdl3_image)

# ================= SDL_ttf ExternalProject ==================
ExternalProject_Add(external_sdl_ttf
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
    GIT_TAG release-3.2.2
    PREFIX ${CMAKE_BINARY_DIR}/external/SDL_ttf
    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${SDL_TTF_INSTALL_DIR}
        -DSDLTTF_SAMPLES=OFF
        -DSDLTTF_TESTS=OFF
        -DSDLTTF_INSTALL_CMAKE_CONFIG=OFF
        -DSDLTTF_INSTALL_PKGCONFIG=OFF
        -DSDLTTF_VENDORED=ON
        -DCMAKE_POSITION_INDEPENDENT_CODE=ON
        -DANDROID_STL=c++_shared
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DBUILD_SHARED_LIBS=ON
        -DSDL3TTF_INSTALL=ON
        -DSDL3_DIR=${SDL3_INSTALL_DIR}/lib/cmake/SDL3
        -DCMAKE_PREFIX_PATH=${SDL3_INSTALL_DIR}
        -DCMAKE_MAKE_PROGRAM=${CMAKE_MAKE_PROGRAM}
    UPDATE_COMMAND ""
    BUILD_BYPRODUCTS ${SDL_TTF_INSTALL_DIR}/lib/libSDL3_ttf.so
    DEPENDS external_sdl3
    CMAKE_CACHE_ARGS
        -DCMAKE_TOOLCHAIN_FILE:PATH=${CMAKE_TOOLCHAIN_FILE}
        -DANDROID_PLATFORM:STRING=${ANDROID_PLATFORM}
        -DANDROID_ABI:STRING=${ANDROID_ABI}
)

# Import SDL3_ttf as IMPORTED
add_library(SDL3_ttf SHARED IMPORTED GLOBAL)
set_target_properties(SDL3_ttf PROPERTIES
    IMPORTED_LOCATION "${SDL_TTF_INSTALL_DIR}/lib/libSDL3_ttf.so"
    INTERFACE_INCLUDE_DIRECTORIES "${SDL_TTF_INSTALL_DIR}/include"
)
add_dependencies(SDL3_ttf external_sdl_ttf)

# Interface wrapper for ttf
add_library(sdl3_ttf_wrapper INTERFACE)
target_include_directories(sdl3_ttf_wrapper INTERFACE "${SDL_TTF_INSTALL_DIR}/include")
target_link_libraries(sdl3_ttf_wrapper INTERFACE SDL3 SDL3_ttf)
add_dependencies(sdl3_ttf_wrapper external_sdl_ttf)

# ================= ImGui ==================
FetchContent_Declare(imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.92.3
)
FetchContent_MakeAvailable(imgui)

# ImGui headers + backends
add_library(imgui_headers INTERFACE)
target_include_directories(imgui_headers INTERFACE
    "${imgui_SOURCE_DIR}"
    "${imgui_SOURCE_DIR}/backends"
)

# ================= spdlog =================
FetchContent_Declare(spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG v1.15.1
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(spdlog)
set_target_properties(spdlog PROPERTIES EXCLUDE_FROM_ALL TRUE SYSTEM TRUE)

add_library(spdlog_headers INTERFACE)
target_include_directories(spdlog_headers INTERFACE
        ${spdlog_SOURCE_DIR}/include
)

# ------------------- single external_prebuild target -------------------
add_custom_target(external_prebuild
    COMMENT "Build external SDL projects and copy .so files to Gradle merged_jni_libs"
    DEPENDS external_sdl3 external_sdl3_image external_sdl_ttf
)

# ensure merged jni out exists before copying
add_custom_command(TARGET external_prebuild PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${MERGED_JNI_OUT}
)

# copy .so files after external projects finished
add_custom_command(TARGET external_prebuild POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${SDL3_INSTALL_DIR}/lib/libSDL3.so"
        "${MERGED_JNI_OUT}/libSDL3.so"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${SDL3_IMAGE_INSTALL_DIR}/lib/libSDL3_image.so"
        "${MERGED_JNI_OUT}/libSDL3_image.so"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${SDL_TTF_INSTALL_DIR}/lib/libSDL3_ttf.so"
        "${MERGED_JNI_OUT}/libSDL3_ttf.so"
    COMMENT "Copied SDL .so files to ${MERGED_JNI_OUT}"
)

# ================= Add JNI folder ==================
add_subdirectory(jni)

# ensure external libs are built/copied before your native target is linked/built
add_dependencies(${CMAKE_PROJECT_NAME} external_prebuild)

# (do not add additional post-builds on the IMPORTED targets)
