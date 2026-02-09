#include "CameraManager.h"

Camera* CameraManager::getCurrentCamera() const
{
    return this->pCamera;
}

// Set window size using width/height (logical size)
void CameraManager::setWindowSize(float width, float height)
{
    this->windowSize = Vector2D(width, height);
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
