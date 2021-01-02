#pragma once
#include "GameObject.h"

#define REWARD_LEVEL_UP_SUPER_LEAF_BBOX_WIDTH  16
#define REWARD_LEVEL_UP_SUPER_LEAF_BBOX_HEIGHT 14
#define REWARD_LEVEL_UP_SUPER_MUSHROOM_BBOX_WIDTH  16
#define REWARD_LEVEL_UP_SUPER_MUSHROOM_BBOX_HEIGHT 16

#define REWARD_LEVEL_UP_LEAF_JUMP_SPEED_Y	0.35f
#define REWARD_LEVEL_UP_MUSHROOM_JUMP_SPEED_Y	0.01f
#define REWARD_LEVEL_UP_SHAKING_SPEED_X	0.06f
#define REWARD_LEVEL_UP_GRAVITY			0.002f
#define REWARD_LEVEL_UP_JET				0.00005f
#define REWARD_LEVEL_UP_WALKING_SPEED	0.07f
#define REWARD_LEVEL_UP_MAX_FALLING_SPEED	0.5f


#define REWARD_LEVEL_UP_STATE_JUMPING			1
#define REWARD_LEVEL_UP_STATE_WALKING_LEFT		2
#define REWARD_LEVEL_UP_STATE_WALKING_RIGHT		3
#define REWARD_LEVEL_UP_STATE_SHAKING_LEFT		4
#define REWARD_LEVEL_UP_STATE_SHAKING_RIGHT		5


#define REWARD_LEVEL_UP_TYPE_SUPER_LEAF		100
#define REWARD_LEVEL_UP_TYPE_SUPER_MUSHROOM	200


#define REWARD_LEVEL_UP_ANI_SUPER_LEAF_LEFT	0
#define REWARD_LEVEL_UP_ANI_SUPER_LEAF_RIGHT		1
#define REWARD_LEVEL_UP_ANI_SUPER_MUSHROOM		2

#define REWARD_LEVEL_UP_ANI_SET_ID			71000

class CReward_LevelUp : public CGameObject
{
private:

	int type;
	float start_y;

	void CalcPotentialCollisionWithMario();
public:
	bool isEnable;
	CReward_LevelUp(float _x, float _y);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	void SetState(int _state);
	bool IsFalling();

	void Update_Leaf();
	void Update_MushRoom(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	bool IsOnTheLeftOfMario();
	void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents);

	int GetType() { return type; }
	void SetType(int _type) { type = _type; }
};

