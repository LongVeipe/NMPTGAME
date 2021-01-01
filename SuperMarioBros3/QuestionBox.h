#pragma once
#include "GameObject.h"

#define QUESTION_BOX_TYPE_COIN		1
#define QUESTION_BOX_TYPE_LEVEL_UP	2
#define QUESTION_BOX_TYPE_LIFE_UP	3

#define QUESTION_BOX_BBOX_WIDTH		16
#define QUESTION_BOX_BBOX_HEIGHT		16

#define QUESTION_BOX_STATE_IDLE		1
#define QUESTION_BOX_STATE_JUMPING	2


#define QUESTION_BOX_ANI_INTACT		0
#define QUESTION_BOX_ANI_EMPTY		1

#define QUESTION_BOX_JUMP_SPEED_Y		0.3f
#define QUESTION_BOX_GRAVITY			0.003f
#define QUESTION_BOX_MAX_FALLING_SPEED	0.5f

class CQuestionBox: public CGameObject
{
private:
	int type;
	float start_y;
	CGameObject* reward;

public:

	bool isEmpty;
	CQuestionBox(float _x, float _y, int _type);
	~CQuestionBox();
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);


	void CreateReward();

	int GetType() { return this->type; }
	void SetState(int _state);

	CGameObject* GetReward() { return this->reward; };
	void SetReward(CGameObject* rew) { this->reward = rew; }

};

