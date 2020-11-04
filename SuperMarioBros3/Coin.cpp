#include "Coin.h"


CCoin::CCoin(float _x, float _y, int _state)
{
	this->x = _x;
	this->y = _y;
	this->start_y = _y;
	this->state = _state;
}

void CCoin::Render()
{
	if (this->state != COIN_STATE_HIDDEN)
	{
		animation_set->at(0)->Render(x, y);
	}
	
	//RenderBoundingBox();
}

void CCoin::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (this->state == COIN_STATE_SHOW_JUMP)
	{
		CGameObject::Update(dt, coObjects);

		vector<LPCOLLISIONEVENT> coEvents;
		vector<LPCOLLISIONEVENT> coEventsResult;

		coEvents.clear();
		CalcPotentialCollisions(coObjects, coEvents);
		if (this->y < start_y - dy)
		{
			this->vy += COIN_GRAVITY * dt;
			y += dy;
		}
		else
		{
			y = start_y;
			ny = 0;
			this->vy = 0;
			this->state = COIN_STATE_HIDDEN;
		}
	}

}

void CCoin::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (this->state == COIN_STATE_HIDDEN)
		l = t = r = b = 0;
	else
	{
		l = x;
		t = y;
		r = x + COIN_BBOX_WIDTH;
		b = y + COIN_BBOX_HEIGHT;
	}
}

void CCoin::SetState(int _state)
{
	CGameObject::SetState(_state);
	switch (state)
	{
	case COIN_STATE_HIDDEN:
		vy = 0;
		ny = 0;
		break;
	case COIN_STATE_SHOW_JUMP:
		vy = -COIN_JUMP_SPEED_Y;
		ny = -1;
		break;

	}
}

