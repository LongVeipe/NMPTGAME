#pragma once
#include "GameObject.h"

#define MPLATFORM_BBOX_WIDTH	48
#define MPLATFORM_BBOX_HEIGHT	16

#define MPLATFORM_STATE_INACTIVE	1
#define MPLATFORM_STATE_MOVING		2
#define MPLATFORM_STATE_FALLING		3

#define MPLATFORM_SPEED_X		0.03f
#define MPLATFORM_GRAVITY		0.0001f

class CMovingPlatform: public CGameObject
{
public:
	bool isBeingTouched = false;
	CMovingPlatform(float _x, float _y);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);

	void SetState(int _state);
};

