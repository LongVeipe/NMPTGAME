#pragma once
#include "Bullet.h"


#define BULLET_MARIO_SPEED_X		0.15f
#define BULLET_MARIO_FIRST_SPEED_Y	0.015f
#define BULLET_MARIO_MAX_FALLING_SPEED	0.15
#define BULLET_MARIO_GRAVITY		0.002
#define BULLET_MARIO_DEFLECT_SPEED	 0.2f

#define BULLET_MARIO_EXPLOSION_TIME	120


class CBullet_Mario : public CBullet
{
private:
	DWORD StartExplode_time;
public:
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void SetState(int _state);
	CBullet_Mario(float x, float y);
	virtual ~CBullet_Mario();
	DWORD GetStartExplode_time() { return StartExplode_time; }
};