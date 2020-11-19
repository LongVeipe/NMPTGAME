#pragma once
#include "GameObject.h"

#define BULLET_BBOX_WIDTH  8
#define BULLET_BBOX_HEIGHT 8

#define BULLET_ANI_FLYING_RIGHT		0
#define BULLET_ANI_FLYING_LEFT		1
#define BULLET_ANI_EXPLODING_RIGHT	2
#define BULLET_ANI_EXPLODING_LEFT	3

#define BULLET_STATE_FLYING		100
#define BULLET_STATE_EXPLODING	200

#define BULLET_SPEED_X		0.15f
#define BULLET_FIRST_SPEED_Y	0.015f
#define BULLET_MAX_FALLING_SPEED	0.15
#define BULLET_GRAVITY		0.002
#define BULLET_DEFLECT_SPEED	 0.2f

#define BULLET_ANIMATION_SET_ID		11000
#define BULLET_EXPLOSION_TIME	120


class CBullet : public CGameObject
{
private:
	DWORD StartExplode_time;
public:
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int _state);
	CBullet(float x, float y);
	virtual ~CBullet();
	//bool IsCollision;
	DWORD GetStartExplode_time() { return StartExplode_time; }
};
typedef CBullet* LPBULLET;