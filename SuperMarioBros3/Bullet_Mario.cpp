#include "Bullet_Mario.h"
#include "Brick.h"
#include "Mario.h"
#include "PlayScence.h"
#include "Utils.h"

CBullet_Mario::CBullet_Mario(float _x, float _y) :CBullet(_x,_y)
{
	state = BULLET_STATE_FLYING;
	StartExplode_time = (DWORD)0;
}



void CBullet_Mario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	vy += BULLET_MARIO_GRAVITY * dt;
	if (vy > BULLET_MARIO_MAX_FALLING_SPEED)
		vy = BULLET_MARIO_MAX_FALLING_SPEED;


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
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
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
				float bleft, btop, bright, bbottom;
				GetBoundingBox(bleft, btop, bright, bbottom);
				float oleft, otop, obottom, oright;
				e->obj->GetBoundingBox(oleft, otop, oright, obottom);
				if (e->nx != 0)
				{
					if (brick->GetType() != BRICK_TYPE_BIG_BLOCK)
					{
						//this->vx = 0;
						this->x = x0 + min_tx * this->dx + e->nx * 0.1f;
						SetState(BULLET_STATE_EXPLODING);
					}

				}
				else if (e->ny != 0)
				{
					this->y = y0 + min_ty * this->dy + e->ny * 0.1f;
					this->vy = -BULLET_MARIO_DEFLECT_SPEED;
				}
			}
			else if(dynamic_cast<CGoomba*>(e->obj))
			{
				CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
				goomba->SetState(GOOMBA_STATE_DIE_X);
				this->SetState(BULLET_STATE_EXPLODING);
			}
			else if(dynamic_cast<CMario*>(e->obj))
			{
				CMario* mario = dynamic_cast<CMario*>(e->obj);
				mario->BeDamaged();
			}

		}

	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}



void CBullet_Mario::SetState(int _state)
{
	CGameObject::SetState(_state);
	switch (_state)
	{
	case BULLET_STATE_EXPLODING:
		vx = vy = 0;
		StartExplode_time =(DWORD) GetTickCount64();
		break;
	}
}
CBullet_Mario::~CBullet_Mario()
{

}

