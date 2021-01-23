#include "Coin.h"
#include "Mario.h"
#include "PlayScence.h"
#include "PointsEffect.h"


CCoin::CCoin(float _x, float _y)
{
	this->x = _x;
	this->y = _y;
	this->start_y = _y;
	IsEnable = true;
	type = COIN_TYPE_DEFAULT;
}
void CCoin::Render()
{
	if (!IsInCamera())
		return;
	if (IsEnable)
	{
		animation_set->at(0)->Render(x, y);
	}
}

void CCoin::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (!IsInCamera() || !IsEnable)
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
		IsEnable = false;
		CPointsEffect* pe = new CPointsEffect(x, y, POINTS_100);
		CPointsEffects::GetInstance()->Add(pe);
	}
	CalcPotentialCollisionWithMario();
}
void CCoin::CalcPotentialCollisionWithMario()
{
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	float ml, mt, mr, mb;
	mario->GetBoundingBox(ml, mt, mr, mb);
	float cl, ct, cr, cb;
	GetBoundingBox(cl, ct, cr, cb);

	if (cb<mt || ct>mb || ml > cr || mr < cl)
		return;
	if ((cl<ml && cr>ml) || (cl < mr && mr < cr))
	{
		IsEnable = false;
		mario->UpMoney();
		if (type == COIN_TYPE_GOLD_BOX)
			mario->UpPoints(50);
		else
			mario->UpPoints(100);
	}
}

void CCoin::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (!IsEnable)
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

