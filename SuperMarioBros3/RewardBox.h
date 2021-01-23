#pragma once
#include "GameObject.h"

#define REWARD_BOX_TYPE_REWARD_COIN		1
#define REWARD_BOX_TYPE_REWARD_LEVEL_UP	2
#define REWARD_BOX_TYPE_REWARD_LIFE_UP	3
#define REWARD_BOX_TYPE_REWARD_SWITCH	4
#define REWARD_BOX_TYPE_REWARD_COINS	5

#define REWARD_BOX_TYPE_QUESTION	1
#define REWARD_BOX_TYPE_GOLD		2

#define REWARD_BOX_BBOX_WIDTH		16
#define REWARD_BOX_BBOX_HEIGHT		16

#define REWARD_BOX_STATE_IDLE		1
#define REWARD_BOX_STATE_JUMPING	2


#define REWARD_BOX_ANI_QUESTION_INTACT	0
#define REWARD_BOX_ANI_EMPTY			1
#define REWARD_BOX_ANI_GOLD_INTACT		2

#define REWARD_BOX_JUMP_SPEED_Y		0.3f
#define REWARD_BOX_GRAVITY			0.003f
#define REWARD_BOX_MAX_FALLING_SPEED	0.3f

#define REWARD_HIDING_TIME		7000
#define REWARD_BOX_MAX_COIN_STACK	10

class CRewardBox : public CGameObject
{
private:
	int type;
	int rewardType;
	float start_y;
	CGameObject* reward;
	DWORD hide_start;
	int coinStack = REWARD_BOX_MAX_COIN_STACK;

	void CalculateBeSwingedTail();
public:
	bool isEnable;
	bool isHiding;
	bool isEmpty;
	bool isBeingBroken;
	CRewardBox(float _x, float _y, int _type, int _rewardType);
	~CRewardBox();
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);


	void Update_GoldBox(vector<LPGAMEOBJECT>* coObjects);
	void BeAttacked();
	void BeBroken();
	void Hide();
	void CreateReward();
	void UpdateFlag();

	int GetType() { return this->type; }
	void SetState(int _state);
	int GetRewardType() { return rewardType; }
	void SetRewardType(int _rewardType) { rewardType = _rewardType; }

	CGameObject* GetReward() { return this->reward; };
	void SetReward(CGameObject* rew) { this->reward = rew; }

};
