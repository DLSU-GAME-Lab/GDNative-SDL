#pragma once
#include "AGameObject.h"

class GUIUtils
{
public:
	static void setGUITopLeft(AGameObject* pObject, Vector2D offset = { 0, 0 });
	static void setGUITopCenter(AGameObject* pObject, Vector2D offset = { 0, 0 });
	static void setGUITopRight(AGameObject* pObject, Vector2D offset = { 0, 0 });

	static void setGUIMidLeft(AGameObject* pObject, Vector2D offset = { 0, 0 });
	static void setGUIMidCenter(AGameObject* pObject, Vector2D offset = { 0, 0 });
	static void setGUIMidRight(AGameObject* pObject, Vector2D offset = { 0, 0 });

	static void setGUIBotLeft(AGameObject* pObject, Vector2D offset = { 0, 0 });
	static void setGUIBotCenter(AGameObject* pObject, Vector2D offset = { 0, 0 });
	static void setGUIBotRight(AGameObject* pObject, Vector2D offset = { 0, 0 });

private:
	static void setSpritePivot(AGameObject* pObject, Vector2D pivot);
};

