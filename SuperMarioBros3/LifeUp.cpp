#include "LifeUp.h"
#include "Scence.h"
#include "PlayScence.h"
#include "RewardBox.h"
#include <algorithm>
#include "PointsEffect.h"
#include "IntroScene.h"



CLifeUp::CLifeUp(float _x, float _y)
{
	this->x = _x;
	this->y = _y;
	this->start_y = _y;
	IsEnable = true;
}
void CLifeUp::Render()
{
	if (!IsEnable)
		return;
	CSprites::GetInstance()->Get(LIFEUP_SPRITE_ID)->Draw(x,y - HUD_HEIGHT);
}
void CLifeUp::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = l + LIFEUP_BBOX_WIDTH;
	b = t + LIFEUP_BBOX_HEIGHT;
}
void CLifeUp::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (!IsEnable)
		return;

	CGameObject::Update(dt, coObjects);
	CalcPotentialCollisionWithMario();
	if (state == LIFEUP_STATE_JUMPING)
	{
		if (y + LIFEUP_BBOX_HEIGHT >= start_y)
		{
			y += dy;
		}
		else
		{
			if (IsOnTheLeftOfMario())
				SetState(LIFEUP_STATE_WALKING_LEFT);
			else
				SetState(LIFEUP_STATE_WALKING_RIGHT);
		}
		return;
	}
	vy += LIFEUP_GRAVITY * dt;
	if (vy > LIFEUP_MAX_FALLING_SPEED)
		vy = LIFEUP_MAX_FALLING_SPEED;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	this->CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{

		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		float x0 = x;
		float y0 = y;
		x += dx;
		y += dy;
		if (nx != 0)
		{
			this->x = x0 + min_tx * this->dx + nx * 0.1f;
			this->vx = -vx;
		}
		if (ny != 0)
		{
			this->vy = 0;
			this->y = y0 + min_ty * this->dy + ny * 0.1f;
			IsTouchingGround = true;
		}

	}

	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CLifeUp::SetState(int _state)
{
	CGameObject::SetState(_state);
	switch (state)
	{
	case LIFEUP_STATE_WALKING_RIGHT:
		vx = LIFEUP_WALKING_SPEED;
		break;
	case LIFEUP_STATE_WALKING_LEFT:
		vx = -LIFEUP_WALKING_SPEED;
		break;
	case LIFEUP_STATE_JUMPING:
		vy = -LIFEUP_JUMP_SPEED_Y;
		break;
	}
}

bool CLifeUp::IsOnTheLeftOfMario()
{
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	float ml, mt, mr, mb;
	float rl, rt, rr, rb;
	mario->GetBoundingBox(ml, mt, mr, mb);
	GetBoundingBox(rl, rt, rr, rb);
	float mmid = (ml + mr) / 2;
	float rmid = (rl + rr) / 2;
	if (rmid > mmid)
	{
		return false;
	}
	return true;
}
void CLifeUp::CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents)
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		CScene* s = CGame::GetInstance()->GetCurrentScene();
		if (dynamic_cast<CPlayScene*>(s))
			if (!IsInCamera())
				continue;

		LPGAMEOBJECT object = coObjects->at(i);
		if (!dynamic_cast<CBrick*>(object) && !dynamic_cast<CRewardBox*>(object))
			continue;


		LPCOLLISIONEVENT e = SweptAABBEx(object);

		if (e->t > 0 && e->t <= 1.0f)
		{
			float mleft, mtop, mright, mbottom;
			GetBoundingBox(mleft, mtop, mright, mbottom);
			float oleft, otop, obottom, oright;
			e->obj->GetBoundingBox(oleft, otop, oright, obottom);
			if (e->nx != 0)
			{
				if (ceil(mbottom) == otop)
				{
					continue;
				}
			}
			coEvents.push_back(e);
		}
		else
			delete e;
	}

	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);
}
void CLifeUp::CalcPotentialCollisionWithMario()
{
	CScene* s = CGame::GetInstance()->GetCurrentScene();
	CMario* mario;
	if (dynamic_cast<CPlayScene*>(s))
	{
		mario = ((CPlayScene*)s)->GetPlayer();
		LPCOLLISIONEVENT e = SweptAABBEx(mario);

		if (e->t > 0 && e->t <= 1.0f)
		{
			IsEnable = false;
			mario->UpLife();
			CPointsEffect* pe = new CPointsEffect(x, y, POINTS_1VP);
			CPointsEffects::GetInstance()->Add(pe);
		}
	}
}