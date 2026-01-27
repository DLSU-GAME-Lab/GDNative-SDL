#include "CameraManager.h"
#include "Settings.h"

Camera* CameraManager::getCurrentCamera() const
{
    return this->pCamera;
}

void CameraManager::setWindowSize(SDL_Window* pWindow)
{
    this->windowSize = Vector2D(gameWidth, gameHeight);
    if (this->pCamera) {
        this->pCamera->setWindowSize(this->windowSize);
    }
}

Vector2D CameraManager::getWindowSize() const
{
    return this->windowSize;
}

/* * * * * * * * * * * * * * * * * * * * *
 *       SINGLETON-RELATED CONTENT       *
 * * * * * * * * * * * * * * * * * * * * */
CameraManager* CameraManager::P_SHARED_INSTANCE = NULL;

void CameraManager::initialize()
{
    P_SHARED_INSTANCE = new CameraManager();
    P_SHARED_INSTANCE->pCamera = new Camera();

    // set initial logical window size so getHalfWidth/getHalfHeight are correct immediately
    P_SHARED_INSTANCE->windowSize = Vector2D(gameWidth, gameHeight);
    P_SHARED_INSTANCE->pCamera->setWindowSize(P_SHARED_INSTANCE->windowSize);
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
