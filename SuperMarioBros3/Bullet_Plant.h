#pragma once
#include "Bullet.h"

#define BULLET_PLANT_SPEED	0.05f
class CBullet_Plant: public CBullet
{
public:
	void Calculate_vxvy(int angle);
	
	void CalcPotentialCollisionWithMario();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	CBullet_Plant(float x, float y, int angle);
	virtual ~CBullet_Plant();
};

