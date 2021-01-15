#include "MarioWM.h"
#include "Utils.h"

CMarioWM::CMarioWM(float _x , float _y ):CGameObject::CGameObject()
{
	x = _x;
	y = _y;
	level = MARIOWM_LEVEL_SMALL;
	canWalkToRight = true;
	SetState(MARIOWM_STATE_IDLE);
}

void CMarioWM::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	if (IsBeingPrevented && GetTickCount64() - bePrevented_start > MARIOWM_BE_PREVENTED_TIME)
		IsBeingPrevented = false;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);

	// No collision occured, proceed normally
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
		float x0 = x, y0 = y;
		x = x0 + dx;
		y = y0 + dy;

		float mleft, mtop, mright, mbottom;
		GetBoundingBox(mleft, mtop, mright, mbottom);
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<CStation*>(e->obj))
			{
				CStation* station = dynamic_cast<CStation*>(e->obj);
				bool l, t, r, b;
				station->GetWay(l, t, r, b);
				if ((l && nx < 0) || (r && nx > 0) || (t && ny < 0) || (b && ny > 0))
				{
					station->IsReadyToPrevent = true;
					station->GetWay(canWalkToLeft, canWalkUp, canWalkToRight, canWalkDown);

				}
			}
		}
	}
}
void CMarioWM::Render()
{
	int ani = -1;
	switch (level)
	{
	case MARIOWM_LEVEL_SMALL:
		ani = MARIOWM_ANI_SMALL;
		break;
	case MARIOWM_LEVEL_BIG:
		ani = MARIOWM_ANI_BIG;
		break;
	case MARIOWM_LEVEL_RACCOON:
		ani = MARIOWM_ANI_RACCOON;
		break;
	case MARIOWM_LEVEL_FIRE:
		ani = MARIOWM_LEVEL_FIRE;
		break;
	}
	animation_set->at(ani)->Render(x, y);
}

void CMarioWM::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	switch (level)
	{
	case MARIOWM_LEVEL_SMALL:
		right = left + MARIOWM_BBOX_SMALL_WIDTH;
		bottom = top + MARIOWM_BBOX_SMALL_HEIGHT;
		break;
	case MARIOWM_LEVEL_BIG:
		right = left + MARIOWM_BBOX_BIG_WIDTH;
		bottom = top + MARIOWM_BBOX_BIG_HEIGHT;
		break;
	case MARIOWM_LEVEL_RACCOON:
		right = left + MARIOWM_BBOX_RACCOON_WIDTH;
		bottom = top + MARIOWM_BBOX_RACCOON_HEIGHT;
		break;
	case MARIOWM_LEVEL_FIRE:
		right = left + MARIOWM_BBOX_FIRE_WIDTH;
		bottom = top + MARIOWM_BBOX_FIRE_HEIGHT;
		break;
	}
}
void CMarioWM::SetState(int _state)
{
	if (state != MARIOWM_STATE_IDLE && _state!= MARIOWM_STATE_IDLE)
		return;
	switch (_state)
	{
	case MARIOWM_STATE_IDLE:
		vx = 0;
		vy = 0;
		CGameObject::SetState(_state);
		break;
	case MARIOWM_STATE_WALKING_UP:
		if (canWalkUp)
		{
			vy = -MARIOWM_WALKING_SPEED;
			CGameObject::SetState(_state);
		}
		break;
	case MARIOWM_STATE_WALKING_DOWN:
		if (canWalkDown)
		{
			vy = MARIOWM_WALKING_SPEED;
			CGameObject::SetState(_state);
		}
		break;
	case MARIOWM_STATE_WALKING_LEFT:
		if (canWalkToLeft)
		{
			vx = -MARIOWM_WALKING_SPEED;
			CGameObject::SetState(_state);
		}
		break;
	case MARIOWM_STATE_WALKING_RIGHT:
		if (canWalkToRight)
		{
			vx = MARIOWM_WALKING_SPEED;
			CGameObject::SetState(_state);
		}
		break;
	}
}

void CMarioWM::BePrevented()
{
	SetState(MARIOWM_STATE_IDLE);
	IsBeingPrevented = true;
	bePrevented_start = GetTickCount64();
}
