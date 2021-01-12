#pragma once
#include "GameObject.h"
#include "Sprites.h"

#define CURTAIN_SPRITE_ID	20001
#define CURTAIN_SPEED_Y		0.1

#define CURTAIN_HEIGHT		187
class CCurtain: public CGameObject
{
	LPSPRITE sprite;
public:

	bool isFinish;

	CCurtain();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
};

