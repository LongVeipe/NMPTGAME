#include "QuestionBox.h"
#include "Coin.h"
#include "Reward_LevelUp.h"

CQuestionBox::CQuestionBox(float _x, float _y, int _type)
{
	this->type = _type;
	this->start_y = _y;

	isEmpty = false;
	SetState(QUESTION_BOX_STATE_IDLE);
}

CQuestionBox::~CQuestionBox()
{
	delete reward;
	reward = nullptr;
}


void CQuestionBox::Render()
{
	if (!IsInCamera())
		return;
	int ani = -1;
	if (isEmpty)
		ani = QUESTION_BOX_ANI_EMPTY;
	else
		ani = QUESTION_BOX_ANI_INTACT;
	animation_set->at(ani)->Render(x, y);

	//render reward
	if (reward != nullptr)
	{
		reward->Render();
	}
}

void CQuestionBox::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isEmpty && state != QUESTION_BOX_STATE_IDLE)
	{
		CGameObject::Update(dt, coObjects);

		if (this->y < start_y - dy)
		{
			this->vy += QUESTION_BOX_GRAVITY * dt;
			if (vy > QUESTION_BOX_MAX_FALLING_SPEED)
				vy = QUESTION_BOX_MAX_FALLING_SPEED;
			y += dy;
		}
		else
		{
			y = start_y;
			SetState(QUESTION_BOX_STATE_IDLE);
		}

	}
	if(reward != nullptr)
		reward->Update(dt, coObjects);
}

void CQuestionBox::CreateReward()
{
	switch (type)
	{
	case QUESTION_BOX_TYPE_COIN:
		reward = new CCoin(x, y - 24);
		break;
	case QUESTION_BOX_TYPE_LEVEL_UP:
		reward = new CReward_LevelUp(x, y);
		break;
	case QUESTION_BOX_TYPE_LIFE_UP:
		break;
	}
}

void CQuestionBox::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + QUESTION_BOX_BBOX_WIDTH;
	b = y + QUESTION_BOX_BBOX_HEIGHT;
}

void CQuestionBox::SetState(int _state)
{
	CGameObject::SetState(_state);
	switch (state)
	{
	case QUESTION_BOX_STATE_IDLE:
		vy = 0;
		break;
	case QUESTION_BOX_STATE_JUMPING:
		vy = -QUESTION_BOX_JUMP_SPEED_Y;
		break;
	}
}
