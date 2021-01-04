#include "Coin.h"
#include "Mario.h"
#include "PlayScence.h"


CCoin::CCoin(float _x, float _y)
{
	this->x = _x;
	this->y = _y;
	this->start_y = _y;
	isEnable = true;
}
void CCoin::Render()
{
	if (!IsInCamera())
		return;
	if (isEnable)
	{
		animation_set->at(0)->Render(x, y);
	}
	else
		isEnable = false;
}

void CCoin::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (!IsInCamera() || !isEnable)
		return;
	CGameObject::Update(dt, coObjects);
	if (state != COIN_STATE_JUMPING)
	{
		CalcPotentialCollisionWithMario();
		return;
	}
	this->vy += COIN_GRAVITY * dt;
	y += dy;
	if(y > start_y)
	{
		y = start_y;
		this->vy = 0;
		isEnable = false;
	}
	CalcPotentialCollisionWithMario();
}
void CCoin::CalcPotentialCollisionWithMario()
{
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	LPCOLLISIONEVENT e = SweptAABBEx(mario);

	if (e->t > 0 && e->t <= 1.0f)
	{
		isEnable = false;
	}
}

void CCoin::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (!isEnable)
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
	case COIN_STATE_JUMPING:
		vy = -COIN_JUMP_SPEED_Y;
		break;
	/*case COIN_STATE_IDLE:
		vy = 0;
		break;*/
	}
}

