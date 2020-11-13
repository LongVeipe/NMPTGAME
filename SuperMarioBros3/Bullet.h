#pragma once
#include "GameObject.h"

#define BULLET_BBOX_WIDTH  8
#define BULLET_BBOX_HEIGHT 8

#define BULLET_ANI_RIGHT	0
#define BULLET_ANI_LEFT		1

#define BULLET_SPEED_X		0.18f
#define BULLET_FIRST_SPEED_Y	0.01f
#define BULLET_GRAVITY		0.0003

#define BULLET_ANIMATION_SET_ID		11000


class CBullet : public CGameObject
{
public:
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	CBullet(float x, float y);
	virtual ~CBullet();
	bool IsCollision;
};
typedef CBullet* LPBULLET;