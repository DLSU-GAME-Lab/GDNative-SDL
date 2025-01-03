LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := main

SDL_PATH := ../SDL
SDL_IMAGE_PATH := ../SDL_image
SDL_MIXER_PATH := ../SDL_mixer

LOCAL_C_INCLUDES := $(LOCAL_PATH)/$(SDL_PATH)/include $(LOCAL_PATH)/$(SDL_IMAGE_PATH)/include $(LOCAL_PATH)/$(SDL_MIXER_PATH)/include $(LOCAL_PATH)/include
# Add your application source files here...

#LOCAL_SRC_FILES := $(subst $(LOCAL_PATH)/,,$(wildcard $(LOCAL_PATH)/src/*.cpp))
LOCAL_SRC_FILES :=  $(LOCAL_PATH)/src/main.cpp \
    $(LOCAL_PATH)/src/AboutMenu.cpp \
    $(LOCAL_PATH)/src/Beetle.cpp \
    $(LOCAL_PATH)/src/Block.cpp \
    $(LOCAL_PATH)/src/BlockDebris.cpp \
    $(LOCAL_PATH)/src/Bowser.cpp \
    $(LOCAL_PATH)/src/Bubble.cpp \
    $(LOCAL_PATH)/src/BulletBill.cpp \
    $(LOCAL_PATH)/src/BulletBillSpawner.cpp \
    $(LOCAL_PATH)/src/CFG.cpp \
    $(LOCAL_PATH)/src/Cheep.cpp \
    $(LOCAL_PATH)/src/CheepSpawner.cpp \
    $(LOCAL_PATH)/src/Coin.cpp \
    $(LOCAL_PATH)/src/Console.cpp \
    $(LOCAL_PATH)/src/GDCore.cpp \
    $(LOCAL_PATH)/src/Event.cpp \
    $(LOCAL_PATH)/src/Fire.cpp \
    $(LOCAL_PATH)/src/FireBall.cpp \
    $(LOCAL_PATH)/src/Flag.cpp \
    $(LOCAL_PATH)/src/Flower.cpp \
    $(LOCAL_PATH)/src/GDTestClass.cpp \
    $(LOCAL_PATH)/src/Goombas.cpp \
    $(LOCAL_PATH)/src/Hammer.cpp \
    $(LOCAL_PATH)/src/HammerBro.cpp \
    $(LOCAL_PATH)/src/IMG.cpp \
    $(LOCAL_PATH)/src/Koppa.cpp \
        $(LOCAL_PATH)/src/Lakito.cpp \
        $(LOCAL_PATH)/src/LevelEditor.cpp \
        $(LOCAL_PATH)/src/LevelText.cpp \
        $(LOCAL_PATH)/src/LoadingMenu.cpp \
        $(LOCAL_PATH)/src/MainMenu.cpp \
        $(LOCAL_PATH)/src/Map.cpp \
        $(LOCAL_PATH)/src/MapLevel.cpp \
        $(LOCAL_PATH)/src/Menu.cpp \
        $(LOCAL_PATH)/src/MenuManager.cpp \
        $(LOCAL_PATH)/src/MenuOption.cpp \
        $(LOCAL_PATH)/src/Minion.cpp \
        $(LOCAL_PATH)/src/Mushroom.cpp \
        $(LOCAL_PATH)/src/Music.cpp \
        $(LOCAL_PATH)/src/MusicManager.cpp \
        $(LOCAL_PATH)/src/OptionsMenu.cpp \
        $(LOCAL_PATH)/src/PauseMenu.cpp \
        $(LOCAL_PATH)/src/Pipe.cpp \
        $(LOCAL_PATH)/src/Plant.cpp \
        $(LOCAL_PATH)/src/Platform.cpp \
        $(LOCAL_PATH)/src/Player.cpp \
        $(LOCAL_PATH)/src/PlayerFireBall.cpp \
        $(LOCAL_PATH)/src/Points.cpp \
        $(LOCAL_PATH)/src/Spikey.cpp \
        $(LOCAL_PATH)/src/Spring.cpp \
        $(LOCAL_PATH)/src/Sprite.cpp \
        $(LOCAL_PATH)/src/Squid.cpp \
        $(LOCAL_PATH)/src/Star.cpp \
        $(LOCAL_PATH)/src/Text.cpp \
        $(LOCAL_PATH)/src/Texture.cpp \
        $(LOCAL_PATH)/src/Toad.cpp \
        $(LOCAL_PATH)/src/UpFire.cpp \
        $(LOCAL_PATH)/src/Vector2.cpp \
        $(LOCAL_PATH)/src/Vine.cpp



LOCAL_SHARED_LIBRARIES := SDL2 SDL2_image SDL2_mixer

LOCAL_LDLIBS := -lGLESv1_CM -lGLESv2 -lOpenSLES -llog -landroid

include $(BUILD_SHARED_LIBRARY)
