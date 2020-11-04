#pragma once
#include "GameObject.h"

#define COIN_BBOX_WIDTH  10
#define COIN_BBOX_HEIGHT 16


#define COIN_JUMP_SPEED_Y		0.5f
#define COIN_GRAVITY			0.0025f

#define COIN_STATE_HIDDEN		0
#define COIN_STATE_SHOW_IDLE	1
#define COIN_STATE_SHOW_JUMP	2

#define COIN_ANI_HIDDEN	100
#define COIN_ANI_SHOW	200


class CCoin : public CGameObject
{
private:
	float start_y;
public:
	CCoin(float _x, float _y, int _state);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void SetState(int _state);
};