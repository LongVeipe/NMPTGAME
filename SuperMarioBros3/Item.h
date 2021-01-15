#pragma once
#include "GameObject.h"

#define ITEM_ANI_STAR		0
#define ITEM_ANI_MUSHROOM	1
#define ITEM_ANI_FLOWER		2

#define ITEM_TYPE_STAR		1
#define ITEM_TYPE_MUSHROOM	2
#define ITEM_TYPE_FLOWER	3

#define ITEM_BBOX_WIDTH		16
#define ITEM_BBOX_HEIGHT	16

#define ITEM_CHANGE_TYPE_TIME	100

#define ITEM_SPEED_Y		0.15

class CItem: public CGameObject
{
private:
	int type;
	DWORD changeTypeTime;

public:

	bool IsIntact;
	CItem();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	int GetType() { return type; }

	void UpdateType(DWORD dt);
	void BeTaken();
};

