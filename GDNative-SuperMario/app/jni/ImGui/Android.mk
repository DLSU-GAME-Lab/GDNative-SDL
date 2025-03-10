IMGUI_LOCAL_PATH := $(call my-dir)

###########################
#
# ImGui static library
#
###########################

include $(CLEAR_VARS)

LOCAL_MODULE := ImGui

LOCAL_C_INCLUDES := $(IMGUI_LOCAL_PATH)/include \
                    $(IMGUI_LOCAL_PATH)/../SDL/include

LOCAL_SRC_FILES := $(IMGUI_LOCAL_PATH)/src/imgui.cpp \
                   $(IMGUI_LOCAL_PATH)/src/imgui_demo.cpp \
                   $(IMGUI_LOCAL_PATH)/src/imgui_draw.cpp \
                   $(IMGUI_LOCAL_PATH)/src/imgui_tables.cpp \
                   $(IMGUI_LOCAL_PATH)/src/imgui_widgets.cpp \
                   $(IMGUI_LOCAL_PATH)/src/imgui_impl_sdl2.cpp \
                   $(IMGUI_LOCAL_PATH)/src/imgui_impl_sdlrenderer2.cpp

LOCAL_EXPORT_C_INCLUDES := $(IMGUI_LOCAL_PATH)/include

include $(BUILD_STATIC_LIBRARY)