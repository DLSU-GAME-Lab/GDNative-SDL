#include "GameCamera.h"
#include "Camera.h"

GameCamera::GameCamera() : AGameObject("Game Camera")
{

}

GameCamera::~GameCamera()
{

}

void GameCamera::initialize()
{
	Camera* cam = new Camera("Camera");
	this->attachComponent(cam);
}
