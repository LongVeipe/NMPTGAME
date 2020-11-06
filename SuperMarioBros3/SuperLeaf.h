#pragma once
#include "GameObject.h"

#define SUPER_LEAF_BBOX_WIDTH  16
#define SUPER_LEAF_BBOX_HEIGHT 14


#define SUPER_LEAF_JUMP_SPEED_Y	0.35f
#define SUPER_LEAF_WOBBLE_SPEED_X	0.06f
#define SUPER_LEAF_GRAVITY		0.002f
#define SUPER_LEAF_LIFT			0.00005f

#define SUPER_LEAF_STATE_HIDDEN		0
#define SUPER_LEAF_STATE_JUMP		1
#define SUPER_LEAF_STATE_FALL		2

#define SUPER_LEAF_ANI_SET	2000


class CSuperLeaf : public CGameObject
{
private:

	float start_y;
public:
	CSuperLeaf(float _x, float _y, int _state);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void SetState(int _state);
	bool IsFalling();

};

