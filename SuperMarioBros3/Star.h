#pragma once
#include "GameObject.h"


#define STAR_BBOX_WIDTH	16
#define STAR_BBOX_HEIGHT	16

#define STAR_JUMP_SPEED_Y	0.4f
#define STAR_SPEED_X		0.1f
#define STAR_GRAVITY		0.0012f
#define STAR_MAX_FALL_SPEED	0.1f

#define STAR_STATE_JUMP_RIGHT	1

class CStar: public CGameObject
{

public:
	bool IsTouchingGround = false;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	void SetState(int state);
};

