LOCAL_PATH := $(call my-dir)

###########################
#
# WAVPACK static library
#
###########################

include $(CLEAR_VARS)

LOCAL_MODULE := wavpack

LOCAL_C_INCLUDES := $(LOCAL_PATH)/include
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_C_INCLUDES)

LOCAL_SRC_FILES := \
    src/write_words.c \
    src/unpack3_seek.c \
    src/unpack3_open.c \
    src/unpack3.c \
    src/unpack_utils.c \
    src/unpack_seek.c \
    src/unpack_floats.c \
    src/unpack_dsd.c \
    src/unpack.c \
    src/tags.c \
    src/tag_utils.c \
    src/read_words.c \
    src/pack_utils.c \
    src/pack_floats.c \
    src/pack_dsd.c \
    src/pack_dns.c \
    src/pack.c \
    src/open_utils.c \
    src/open_raw.c \
    src/open_legacy.c \
    src/open_filename.c \
    src/extra2.c \
    src/extra1.c \
    src/entropy_utils.c \
    src/decorr_utils.c \
    src/common_utils.c

ifeq ($(TARGET_ARCH_ABI),x86)
    LOCAL_SRC_FILES += src/pack_x86.S
    LOCAL_CFLAGS += -DOPT_ASM_X86
else ifeq ($(TARGET_ARCH_ABI),x86_64)
    LOCAL_SRC_FILES += src/pack_x64.S
    LOCAL_CFLAGS += -DOPT_ASM_X64
else
    LOCAL_CFLAGS += -DOPT_ASM_ARM
endif

LOCAL_CFLAGS := -DPACKAGE_VERSION=\"5.8.1\" -O3

include $(BUILD_STATIC_LIBRARY)