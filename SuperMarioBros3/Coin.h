#pragma once
#include "GameObject.h"

#define COIN_BBOX_WIDTH  10
#define COIN_BBOX_HEIGHT 16


#define COIN_JUMP_SPEED_Y		0.5f
#define COIN_GRAVITY			0.0025f

#define COIN_STATE_IDLE	1
#define COIN_STATE_JUMPING	2

#define COIN_ANI_SET_ID		70000

class CCoin : public CGameObject
{
private:
	float start_y;
public:
	bool isEnable;
	CCoin(float _x, float _y);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	
	void CalcPotentialCollisionWithMario();

	void SetState(int _state);
};