#include "Coin.h"
#include "Mario.h"
#include "PlayScence.h"
#include "PointsEffect.h"


CCoin::CCoin(float _x, float _y)
{
	this->x = _x;
	this->y = _y;
	this->start_y = _y;
	isEnable = true;
	type = COIN_TYPE_DEFAULT;
}
void CCoin::Render()
{
	if (!IsInCamera())
		return;
	if (isEnable)
	{
		animation_set->at(0)->Render(x, y);
	}
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
		CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
		mario->UpPoints(POINTS_100);
		CPointsEffect* pe = new CPointsEffect(x, y, POINTS_100);
		CPointsEffects::GetInstance()->Add(pe);
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
		mario->UpMoney();
		if (type == COIN_TYPE_GOLD_BOX)
			mario->UpPoints(50);
		else
			mario->UpPoints(100);

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

