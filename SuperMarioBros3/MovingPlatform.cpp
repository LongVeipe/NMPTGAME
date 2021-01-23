#include "MovingPlatform.h"
#include "PlayScence.h"


CMovingPlatform::CMovingPlatform(float _x, float _y): CGameObject()
{
	x = _x;
	y = _y;
	SetState(MPLATFORM_STATE_MOVING);
}
void CMovingPlatform:: Render()
{
	animation_set->at(0)->Render(x, y);
}
void CMovingPlatform::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = l + MPLATFORM_BBOX_WIDTH;
	b = t + MPLATFORM_BBOX_HEIGHT;
}

void CMovingPlatform::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);


	if (this->state == MPLATFORM_STATE_FALLING)
		vy += dt * MPLATFORM_GRAVITY;


	if (isBeingTouched)
	{
		CPlayScene* playScene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
		CMario* mario = playScene->GetPlayer();
		if (mario->IsTouchingGround)
		{
			if (mario->x >= x - MARIO_SMALL_BBOX_WIDTH && mario->x < x + MPLATFORM_BBOX_WIDTH)
			{
				/*if (mario->GetLevel() == MARIO_LEVEL_SMALL)
					mario->y = y  - MARIO_SMALL_BBOX_HEIGHT;
				else
					mario->y = y - MARIO_BIG_BBOX_HEIGHT;*/
				mario->vy = this->vy;
			}
			else
			{
				isBeingTouched = false;
				mario->vy = 0;
			}
		}
		else
		{
			isBeingTouched = false;
		}
	}


	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float x0 = x;
		float y0 = y;
		x += dx;
		y += dy;

		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;
		FilterCollision(coEvents, coEventsResult,min_tx, min_ty,nx, ny, rdx, rdy);

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<CMario*>(e->obj))
			{
				if (nx != 0)
				{

					e->obj->x -= (min_tx * dx + nx * 0.4f);

				}
				else if (e->ny < 0)
				{
					y = y0 + dy;
				}
			}
		}
	}
}


void CMovingPlatform::SetState(int _state)
{
	CGameObject::SetState(_state);
	switch (state)
	{
	case MPLATFORM_STATE_MOVING:
		vx = -MPLATFORM_SPEED_X;
		vy = 0;
		break;
	case MPLATFORM_STATE_FALLING:
		vx = 0;
		break;
	}
}