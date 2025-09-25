#pragma once
#include "AGameObject.h"

class GUIUtils
{
public:
	static void setGUITopLeft(AGameObject* pObject, Vector2D offset = Vector2D());
	static void setGUITopMiddle(AGameObject* pObject, Vector2D offset = Vector2D());
	static void setGUITopRight(AGameObject* pObject, Vector2D offset = Vector2D());

	static void setGUIMidLeft(AGameObject* pObject, Vector2D offset = Vector2D());
	static void setGUIMidMiddle(AGameObject* pObject, Vector2D offset = Vector2D());
	static void setGUIMidRight(AGameObject* pObject, Vector2D offset = Vector2D());

	static void setGUIBotLeft(AGameObject* pObject, Vector2D offset = Vector2D());
	static void setGUIBotMiddle(AGameObject* pObject, Vector2D offset = Vector2D());
	static void setGUIBotRight(AGameObject* pObject, Vector2D offset = Vector2D());

private:
	static void setSpritePivot(AGameObject* pObject, Vector2D pivot);
};

