#pragma once
#include "GameObject.h"

#define LIFEUP_SPRITE_ID	73001

#define LIFEUP_BBOX_WIDTH	16
#define LIFEUP_BBOX_HEIGHT	16

#define LIFEUP_STATE_JUMPING	1
#define LIFEUP_STATE_WALKING_LEFT	2
#define LIFEUP_STATE_WALKING_RIGHT	3

#define LIFEUP_GRAVITY 0.0033f
#define LIFEUP_MAX_FALLING_SPEED	0.5f
#define LIFEUP_WALKING_SPEED	0.07f
#define LIFEUP_JUMP_SPEED_Y	0.01

class CLifeUp: public CGameObject
{
private:
	float start_y;
	void CalcPotentialCollisionWithMario();
public:
	bool IsTouchingGround;
	CLifeUp(float _x, float _y);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	void SetState(int _state);

	bool IsOnTheLeftOfMario();
	void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents);

};

