#pragma once
#include "Camera.h"

class CameraManager
{
private:
    Camera* pCamera = nullptr;
    Vector2D windowSize;
    SDL_Renderer* pRenderer = nullptr;

public:
    Camera* getCurrentCamera() const;
    void setWindowSize(float width, float height);
    //void setWindowSize(SDL_Window* pWindow);
    Vector2D getWindowSize() const;
    void setRenderer(SDL_Renderer* renderer) { pRenderer = renderer; }

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

