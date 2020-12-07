#pragma once
#include "GameObject.h"


#define BULLET_BBOX_WIDTH  8
#define BULLET_BBOX_HEIGHT 8

#define BULLET_STATE_FLYING		100
#define BULLET_STATE_EXPLODING	200

#define BULLET_ANI_FLYING_RIGHT		0
#define BULLET_ANI_FLYING_LEFT		1
#define BULLET_ANI_EXPLODING_RIGHT	2
#define BULLET_ANI_EXPLODING_LEFT	3

#define BULLET_ANIMATION_SET_ID		11000

class CBullet:public CGameObject
{
public:
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Render();
	CBullet(float x, float y);
	virtual ~CBullet();
};

