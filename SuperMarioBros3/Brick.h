#pragma once
#include "GameObject.h"

#define BRICK_BBOX_WIDTH  16
#define BRICK_BBOX_HEIGHT 16

#define BRICK_TYPE_NORMAL 0
#define BRICK_TYPE_BIG_BLOCK 1

class CBrick : public CGameObject
{
private:
	int DetailType;
public:
	CBrick(int _detailType = 0);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	int GetDetailType() { return this->DetailType; }
};