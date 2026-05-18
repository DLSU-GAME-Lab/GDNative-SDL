#pragma once
#include "Camera.h"

class CameraManager
{
private:
    Camera* pCamera = nullptr;
    Vector2D windowSize;
    Vector2D windowScale;

public:
    Camera* getCurrentCamera() const;
    void setWindowSize(SDL_Window* pWindow);
    void setWindowScale(float w, float h);
    Vector2D getWindowSize() const;
    Vector2D getWindowScale() const;
    Vector2D getWindowSizeScaled() const;
    Vector2D getWindowOffset() const;

    /* * * * * * * * * * * * * * * * * * * * *
     *       SINGLETON-RELATED CONTENT       *
     * * * * * * * * * * * * * * * * * * * * */
private:
    static CameraManager* P_SHARED_INSTANCE;

private:
    CameraManager() {};
    CameraManager(const CameraManager&) {};
    CameraManager& operator=(const CameraManager&) {};

public:
    static void initialize();
    static void destroy();

    static CameraManager* getInstance();
    /* * * * * * * * * * * * * * * * * * * * */
};

