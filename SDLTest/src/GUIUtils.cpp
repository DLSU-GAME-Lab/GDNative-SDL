#include "GUIUtils.h"
#include "Settings.h"
#include "SpriteRenderer.h"

void GUIUtils::setGUITopLeft(AGameObject* pObject, Vector2D offset)
{
	pObject->setIsScreenObject(true);
	setSpritePivot(pObject, Vector2D(0.0f, 0.0f));
	pObject->setPos(Vector2D(0.0f, 0.0f) + offset);
}

void GUIUtils::setGUITopCenter(AGameObject * pObject, Vector2D offset)
{
	pObject->setIsScreenObject(true);
	setSpritePivot(pObject, Vector2D(0.5f, 0.0f));
	pObject->setPos(Vector2D(gameWidth * 0.5f, 0.0f) + offset);
}

void GUIUtils::setGUITopRight(AGameObject * pObject, Vector2D offset)
{
	pObject->setIsScreenObject(true);
	setSpritePivot(pObject, Vector2D(1.0f, 0.0f));
	pObject->setPos(Vector2D(gameWidth, 0.0f) + offset);
}

void GUIUtils::setGUIMidLeft(AGameObject * pObject, Vector2D offset)
{
	pObject->setIsScreenObject(true);
	setSpritePivot(pObject, Vector2D(0.0f, 0.5f));
	pObject->setPos(Vector2D(0.0f, gameHeight * 0.5f) + offset);
}

void GUIUtils::setGUIMidCenter(AGameObject * pObject, Vector2D offset)
{
	pObject->setIsScreenObject(true);
	setSpritePivot(pObject, Vector2D(0.5f, 0.5f));
	pObject->setPos(Vector2D(gameWidth * 0.5f, gameHeight * 0.5f) + offset);
}

void GUIUtils::setGUIMidRight(AGameObject * pObject, Vector2D offset)
{
	pObject->setIsScreenObject(true);
	setSpritePivot(pObject, Vector2D(1.0f, 0.5f));
	pObject->setPos(Vector2D(gameWidth, gameHeight * 0.5f) + offset);
}

void GUIUtils::setGUIBotLeft(AGameObject * pObject, Vector2D offset)
{
	pObject->setIsScreenObject(true);
	setSpritePivot(pObject, Vector2D(0.0f, 1.0f));
	pObject->setPos(Vector2D(0.0f, gameHeight) + offset);
}

void GUIUtils::setGUIBotCenter(AGameObject * pObject, Vector2D offset)
{
	pObject->setIsScreenObject(true);
	setSpritePivot(pObject, Vector2D(0.5f, 1.0f));
	pObject->setPos(Vector2D(gameWidth * 0.5f, gameHeight) + offset);
}

void GUIUtils::setGUIBotRight(AGameObject * pObject, Vector2D offset)
{
	pObject->setIsScreenObject(true);
	setSpritePivot(pObject, Vector2D(1.0f, 1.0f));
	pObject->setPos(Vector2D(gameWidth, gameHeight) + offset);
}

void GUIUtils::setSpritePivot(AGameObject* pObject, Vector2D pivot)
{
	SpriteRenderer* pRenderer = (SpriteRenderer*)pObject->findComponentByName("SpriteRenderer");
	if (pRenderer) pRenderer->setPivot(pivot);
}
