#pragma once
#include "GameObject.h"
#include "Font.h"
class CEndSceneNotification: public CGameObject
{
	CFont* font;
	vector<LPSPRITE> course;
	vector<LPSPRITE> clear;
public: 
	CEndSceneNotification(float _x, float _y);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

