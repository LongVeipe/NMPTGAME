#include "Brick.h"


CBrick::CBrick(float _x, float _y, int _Type)
{
	this->Type = _Type;
	this->start_x = _x;
	this->start_y = _y;
	this->Reward = nullptr;
	if (_Type == BRICK_TYPE_QUESTION)
	{
		this->DetailType = BRICK_DETAIL_TYPE_QUESTION_INTACT;
		this->state = BRICK_STATE_IDLE;
	}
}

void CBrick::Render()
{
	int ani = 0;
	if (Type == BRICK_TYPE_QUESTION)
	{
		if (DetailType == BRICK_DETAIL_TYPE_QUESTION_INTACT)
			ani = BRICK_ANI_QUESTION_INTACT;
		else
			ani = BRICK_ANI_QUESTION_EMPTY;
	}
	animation_set->at(ani)->Render(x, y);
	//RenderBoundingBox();
}

void CBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if(this->Type == BRICK_TYPE_QUESTION)
		if (this->DetailType == BRICK_DETAIL_TYPE_QUESTION_EMPTY)
		{
			CGameObject::Update(dt, coObjects);
			
			if (this->y < start_y -dy)
			{
				this->vy += BRICK_GRAVITY * dt;
				y += dy;
			}
			else
			{
				y = start_y;
				ny = 0;
				this->vy = 0;
			}
				
		}
}

void CBrick::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + BRICK_BBOX_WIDTH;
	b = y + BRICK_BBOX_HEIGHT;
}

void CBrick::SetState(int _state)
{
	CGameObject::SetState(_state);
	switch (state)
	{
	case BRICK_STATE_IDLE:
		vy = 0;
		ny = 0;
		break;
	case BRICK_STATE_JUMP:
		vy = -BRICK_JUMP_SPEED_Y;
		ny = -1;
		break;
	
	}
}
