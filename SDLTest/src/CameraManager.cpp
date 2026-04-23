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

void CameraManager::setLetterboxRect(float x, float y, float w, float h)
{
    this->letterboxOffset = Vector2D(x, y);
    this->letterboxScale = Vector2D(w, h);
}

Vector2D CameraManager::getLetterboxOffset() const
{
    return this->letterboxOffset;
}

Vector2D CameraManager::getLetterboxScale() const
{
    return this->letterboxScale;
}

void CameraManager::setActualWindowSize(int x, int y)
{
	this->actualWindowSize = Vector2D(x, y);
}

Vector2D CameraManager::getActualWindowSize() const
{
    return this->actualWindowSize;
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
