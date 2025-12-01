#pragma once
#include "AGameObject.h"

class GUIUtils
{
public:
	static void setGUITopLeft(AGameObject* pObject, Vector2D offset = Vector2D());
	static void setGUITopCenter(AGameObject* pObject, Vector2D offset = Vector2D());
	static void setGUITopRight(AGameObject* pObject, Vector2D offset = Vector2D());

	static void setGUIMidLeft(AGameObject* pObject, Vector2D offset = Vector2D());
	static void setGUIMidCenter(AGameObject* pObject, Vector2D offset = Vector2D());
	static void setGUIMidRight(AGameObject* pObject, Vector2D offset = Vector2D());

	static void setGUIBotLeft(AGameObject* pObject, Vector2D offset = Vector2D());
	static void setGUIBotCenter(AGameObject* pObject, Vector2D offset = Vector2D());
	static void setGUIBotRight(AGameObject* pObject, Vector2D offset = Vector2D());

private:
	static void setSpritePivot(AGameObject* pObject, Vector2D pivot);
};

