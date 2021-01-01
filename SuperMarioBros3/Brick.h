#pragma once
#include "GameObject.h"
#include "Coin.h"

#define BRICK_BBOX_WIDTH  16
#define BRICK_BBOX_HEIGHT 16

#define BRICK_TYPE_PLATFORM		0
#define BRICK_TYPE_BIG_BLOCK	1

#define BRICK_JUMP_SPEED_Y		0.3f
#define BRICK_GRAVITY			0.003f


class CBrick : public CGameObject
{
private:
	int type;
public:
	CBrick(float _x, float _y, int _Type);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);

	int GetType() { return this->type; }
};