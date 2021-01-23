#pragma once
#include"GameObject.h"

#define BROKEN_BRICK_EFFECT_SPRITE_ID		15001

#define BROKEN_BRICK_EFFECT_SPEED_X		0.1f
#define BROKEN_BRICK_EFFECT_SPEED_Y1	0.3f
#define BROKEN_BRICK_EFFECT_SPEED_Y2	0.5f

#define BRICK_FRAGMENT_GRAVITY		0.002f
#define BROKEN_BRICK_EFFECT_APPEAR_TIME	500

class CBrokenBrickEffect :public CGameObject
{
	vector<LPSPRITE> brokenBrickSprite;
	DWORD appear_start = 0;
	float x1, x2;
	float y1, y2;
	float vy1, vy2;
	virtual void GetBoundingBox(float& left, float& top,float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();

public:
	bool IsAppearing = false;

	CBrokenBrickEffect(float x, float y);
	void StartAppear();
};

