#include "SuperLeaf.h"


CSuperLeaf::CSuperLeaf(float _x, float _y, int _state)
{
	this->x = _x;
	this->y = _y;
	this->start_y = _y;
	this->state = _state;
}

void CSuperLeaf::Render()
{
	if (this->state != SUPER_LEAF_STATE_HIDDEN)
	{
		animation_set->at(0)->Render(x, y);
	}

	//RenderBoundingBox();
}

void CSuperLeaf::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	x += dx;
	y += dy;
	if (this->state != SUPER_LEAF_STATE_HIDDEN)
	{
		CGameObject::Update(dt, coObjects);
		
		if (this->state == SUPER_LEAF_STATE_JUMP)
		{
			vy = vy + SUPER_LEAF_GRAVITY*dt + SUPER_LEAF_LIFT*dt;
			if (vy > 0)
			{
				vy = 0;
				vx = SUPER_LEAF_WOBBLE_SPEED_X;
				SetState(SUPER_LEAF_STATE_FALL);
			}
		}
		else
		{
			vy = vy - SUPER_LEAF_LIFT * dt  ;
			if (vy < -( 10* SUPER_LEAF_LIFT * dt))
			{
				vy = SUPER_LEAF_GRAVITY * dt;
				SetState(SUPER_LEAF_STATE_FALL);

			}
		}
	}

}

void CSuperLeaf::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (this->state == SUPER_LEAF_STATE_HIDDEN)
		l = t = r = b = 0;
	else
	{
		l = x;
		t = y;
		r = x + SUPER_LEAF_BBOX_WIDTH;
		b = y + SUPER_LEAF_BBOX_HEIGHT;
	}
}

void CSuperLeaf::SetState(int _state)
{
	CGameObject::SetState(_state);
	switch (state)
	{
	case SUPER_LEAF_STATE_HIDDEN:
		vy = 0;
		ny = 0;
		break;
	case SUPER_LEAF_STATE_JUMP:
		vy = -SUPER_LEAF_JUMP_SPEED_Y;
		ny = -1;
		break;
	case SUPER_LEAF_STATE_FALL:
		vx = -vx;
		ny = 1;
		break;
	}
}

