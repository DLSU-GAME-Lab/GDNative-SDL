#pragma once
#include "Camera.h"

class CameraManager
{
private:
    Camera* pCamera = nullptr;
    Vector2D windowSize;

public:
    Camera* getCurrentCamera() const;
    void setWindowSize(SDL_Window* pWindow);
    Vector2D getWindowSize() const;

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

