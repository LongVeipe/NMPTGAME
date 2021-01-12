#include "Star.h"
#include "Brick.h"
#include "Utils.h"

void CStar::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + STAR_BBOX_WIDTH;
	bottom = y + STAR_BBOX_HEIGHT;
}

void CStar::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (!IsEnable)
		return;
	CGameObject::Update(dt, coObjects);
	vy += STAR_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);
	if (y > 170)
		y = y;
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
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<CBrick*>(e->obj))
			{
				CBrick* brick = dynamic_cast<CBrick*>(e->obj);
				if (e->ny < 0)
				{
					IsTouchingGround = true;
				}

			}
		}
	}
}
void CStar::Render()
{
	if (!IsEnable)
		return;
	animation_set->at(0)->Render(x,y);
}

void CStar::SetState(int _state)
{
	CGameObject::SetState(_state);
	switch (_state)
	{
	case STAR_STATE_JUMP_RIGHT:
		IsTouchingGround = false;
		vx = STAR_SPEED_X;
		vy = -STAR_JUMP_SPEED_Y;
		break;
	}
}
