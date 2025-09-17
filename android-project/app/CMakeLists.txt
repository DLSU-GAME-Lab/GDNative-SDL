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

# =============================================================

# Change to "Release" if Release lmao
set(CMAKE_BUILD_TYPE_INIT "Debug")

# ================= SDL Directories Creation ==================
set(SDL3_INSTALL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/build/SDL3")
file(MAKE_DIRECTORY ${SDL3_INSTALL_DIR}/lib)
file(MAKE_DIRECTORY ${SDL3_INSTALL_DIR}/include)

set(SDL3_IMAGE_INSTALL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/build/SDL3_image")
file(MAKE_DIRECTORY ${SDL3_IMAGE_INSTALL_DIR}/lib)
file(MAKE_DIRECTORY ${SDL3_IMAGE_INSTALL_DIR}/include)

# ================= Add SDL3 via ExternalProject (CMake Stuff =============================
ExternalProject_Add(external_sdl3
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG main  # Choose whatever version/tag here you guys want
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

        # Shared library settings
        -DSDL_SHARED=ON
        -DSDL_STATIC=OFF

        # Enable Android build
        -DSDL_ANDROID=ON
        -DSDL_VULKAN=ON

        # Disable unused features
        -DSDL_HAPTIC=OFF
        -DSDL_POWER=OFF
        -DSDL_OPENGL=OFF
        -DSDL_OPENGLES=OFF
        -DSDL_METAL=OFF

        # Keep essential subsystems
        -DSDL_JOYSTICK=ON
        -DSDL_RENDER=ON
        -DSDL_AUDIO=ON
        -DSDL_VIDEO=ON
        -DSDL_EVENTS=ON
        -DSDL_ATOMIC=ON
        -DSDL_THREADS=ON
        -DSDL_TIMER=ON
        -DSDL_HIDAPI=ON

        # Disable unnecessary dependencies
        -DSDL_DEPS_SHARED=OFF
        -DSDL_EXAMPLES=OFF
        -DSDL_TESTS=OFF

        # Disable unnecessary audio backends
        -DSDL_DUMMYAUDIO=OFF
        -DSDL_DISKAUDIO=OFF

        # Define HIDAPI FLAGS as disabled
        -DCMAKE_C_FLAGS=-DSDL_HIDAPI_DISABLED=1
        -DCMAKE_CXX_FLAGS=-DSDL_HIDAPI_DISABLED=1

        # Do not run configure again if already done
        UPDATE_COMMAND ""
        BUILD_BYPRODUCTS ${SDL3_INSTALL_DIR}/lib/libSDL3.so

        CMAKE_CACHE_ARGS
        -DCMAKE_TOOLCHAIN_FILE:PATH=${CMAKE_TOOLCHAIN_FILE}
        -DANDROID_PLATFORM:STRING=${ANDROID_PLATFORM}
        -DANDROID_ABI:STRING=${ANDROID_ABI}
)
# ====================================================================================

# ===================== Add SDL library ===============================================
add_library(SDL3 SHARED IMPORTED GLOBAL)
set_target_properties(SDL3 PROPERTIES
        IMPORTED_LOCATION "${SDL3_INSTALL_DIR}/lib/libSDL3.so"
        INTERFACE_INCLUDE_DIRECTORIES "${SDL3_INSTALL_DIR}/include"
)

target_compile_definitions(SDL3 INTERFACE
        ANDROID
        __ANDROID__
)

add_dependencies(SDL3 external_sdl3)

add_custom_target(sdl3_prebuild
        COMMAND ${CMAKE_COMMAND} -E echo "Ensuring SDL3 is built first"
        DEPENDS external_sdl3
)
# =====================================================================================

# Add SDL3 wrapper to just get all the headers
add_library(sdl3_wrapper INTERFACE)
target_include_directories(sdl3_wrapper INTERFACE ${SDL3_INSTALL_DIR}/include)
target_link_libraries(sdl3_wrapper INTERFACE SDL3)

# ================= SDL3_image Configuration for Android ==================
ExternalProject_Add(external_sdl3_image
        GIT_REPOSITORY  https://github.com/libsdl-org/SDL_image.git
        GIT_TAG main
        GIT_SHALLOW FALSE
        GIT_PROGRESS TRUE
        GIT_SUBMODULES ""
        PREFIX ${CMAKE_BINARY_DIR}/external/SDL3_image
        CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${SDL3_IMAGE_INSTALL_DIR}
        -DCMAKE_POSITION_INDEPENDENT_CODE=ON
        -DANDROID_STL=c++_shared
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
	-DCMAKE_MAKE_PROGRAM=${CMAKE_MAKE_PROGRAM}

        -DSDL3IMAGE_INSTALL=ON

        # Configure SDL3_image options for Android before fetching
        -DSDL3IMAGE_DEPS_SHARED=OFF
        -DSDL3IMAGE_VENDORED=ON
        -DSDL3IMAGE_BUILD_SHARED_LIBS=OFF

        # Disable image formats that might cause linking issues on Android
        -DSDL3IMAGE_JPG_SHARED=OFF
        -DSDL3IMAGE_PNG_SHARED=OFF
        -DSDL3IMAGE_TIF_SHARED=OFF
        -DSDL3IMAGE_WEBP_SHARED=OFF

        # Enable only basic formats
        -DSDL3IMAGE_JPG=ON
        -DSDL3IMAGE_PNG=ON

        # Disable tests and samples
        -DSDL3IMAGE_SAMPLES=OFF
        -DSDL3IMAGE_TESTS=OFF

        -DSDL3_DIR=${SDL3_INSTALL_DIR}/lib/cmake/SDL3
        -DCMAKE_PREFIX_PATH=${SDL3_INSTALL_DIR}

        # Do not run configure again if already done
        UPDATE_COMMAND ""
        BUILD_BYPRODUCTS ${SDL3_IMAGE_INSTALL_DIR}/lib/libSDL3_image.so

        DEPENDS external_sdl3  # Make sure SDL3 is built first
        CMAKE_CACHE_ARGS
        -DCMAKE_TOOLCHAIN_FILE:PATH=${CMAKE_TOOLCHAIN_FILE}
        -DANDROID_PLATFORM:STRING=${ANDROID_PLATFORM}
        -DANDROID_ABI:STRING=${ANDROID_ABI}
)

# Create wrapper for SDL3_image
add_library(sdl3_image_wrapper INTERFACE)

target_include_directories(sdl3_image_wrapper INTERFACE
        ${SDL3_IMAGE_INSTALL_DIR}/include
)
# Link to the built SDL3_image library
target_link_libraries(sdl3_image_wrapper INTERFACE
        ${SDL3_IMAGE_INSTALL_DIR}/lib/libSDL3_image.so
)
add_dependencies(sdl3_image_wrapper external_sdl3_image)
# ===========================================================================

# Fetch Content spdlog just for logging
# Fetch Content should be the norm when getting stuff from github repos
# I just used ExternalProject for SDL3 because its so big, and it just breaks on FetchContent idfk

FetchContent_Declare(spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG v1.15.1
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(spdlog)
set_target_properties(spdlog PROPERTIES EXCLUDE_FROM_ALL TRUE SYSTEM TRUE) # To remove warnings becoz its from 3rd party (just makes our logs clean)

# Just get spdlog wrapper to only get header files
add_library(spdlog_headers INTERFACE)
target_include_directories(spdlog_headers INTERFACE
        ${spdlog_SOURCE_DIR}/include
)


add_subdirectory(jni)

# Add SDL to Project
add_dependencies(${CMAKE_PROJECT_NAME} sdl3_prebuild)