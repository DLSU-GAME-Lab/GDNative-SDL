#include "CameraManager.h"

Camera* CameraManager::getCurrentCamera() const
{
    return this->pCamera;
}

void CameraManager::setWindowSize(SDL_Window* pWindow)
{

    int w, h;
    SDL_GetWindowSizeInPixels(pWindow, &w, &h);
    this->windowSize.x = w;
    this->windowSize.y = h;
    this->pCamera->setWindowSize(Vector2D(w, h));
}

Vector2D CameraManager::getWindowSize() const
{
    return this->windowSize;
}

void CameraManager::setWindowScale(float w, float h)
{
    this->windowScale.x = w;
    this->windowScale.y = h;
    this->pCamera->setWindowScale(this->windowScale);
}

Vector2D CameraManager::getWindowScale() const
{
    return this->windowScale;
}

Vector2D CameraManager::getWindowSizeScaled() const
{
    return windowSize / windowScale;
}

Vector2D CameraManager::getWindowOffset() const {
    return (windowSize - getWindowSizeScaled()) * 0.5f;
}

/* * * * * * * * * * * * * * * * * * * * *
 *       SINGLETON-RELATED CONTENT       *
 * * * * * * * * * * * * * * * * * * * * */
CameraManager* CameraManager::P_SHARED_INSTANCE = NULL;

void CameraManager::initialize()
{
    P_SHARED_INSTANCE = new CameraManager();
    P_SHARED_INSTANCE->pCamera = new Camera();
}

void CameraManager::destroy()
{
    delete P_SHARED_INSTANCE;
}

CameraManager* CameraManager::getInstance()
{
    return P_SHARED_INSTANCE;
}

/* * * * * * * * * * * * * * * * * * * * */
