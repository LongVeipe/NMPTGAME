#pragma once
#include "GameObject.h"
#include "Coin.h"

#define BRICK_BBOX_WIDTH  16
#define BRICK_BBOX_HEIGHT 16

#define BRICK_TYPE_NORMAL 0
#define BRICK_TYPE_BIG_BLOCK 1
#define BRICK_TYPE_QUESTION 3


#define BRICK_DETAIL_TYPE_QUESTION_INTACT	100
#define BRICK_DETAIL_TYPE_QUESTION_EMPTY	200

#define BRICK_STATE_JUMP	0
#define BRICK_STATE_IDLE	10

#define BRICK_ANI_QUESTION_INTACT 0
#define BRICK_ANI_QUESTION_EMPTY 1

#define BRICK_JUMP_SPEED_Y		0.3f
#define BRICK_GRAVITY			0.003f

#define TYPE_REWARD_COIN			1
#define TYPE_REWARD_SUPER_LEAF		2
#define TYPE_REWARD_SUPER_MUSHROOM	3


class CBrick : public CGameObject
{
private:
	int Type;
	int DetailType;
	float start_x;			
	float start_y;
	CGameObject* Reward;
public:
	CBrick(float _x, float _y, int _Type);
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	int GetDetailType() { return this->DetailType; }
	void SetDetailType(int _detailType) { this->DetailType = _detailType; }
	int GetType() { return this->Type; }
	void SetState(int _state);
	CGameObject* GetReward() { return this->Reward; };
	void SetReward(CGameObject* rew) { this->Reward = rew; }
};