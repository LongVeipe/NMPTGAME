#include "Koopas.h"
#include "Brick.h"
#include "Mario.h"
#include "Game.h"
#include "PlayScence.h"

CKoopas::CKoopas(float _start_x, float _final_x, int _type):CEnemy(_start_x,_final_x,_type)
{

	SetState(KOOPA_STATE_IDLE);
}

void CKoopas::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + KOOPA_BBOX_RED_SMALL_TURTOISESHELL_WIDTH;
	bottom = y + KOOPA_BBOX_RED_SMALL_TURTOISESHELL_HEIGHT;
}

void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CEnemy::Update(dt, coObjects);

	//
	// TO-DO: make sure Koopas can interact with the world and to each of them too!
	// 

	
	if (IsBeingHeld)
	{
		BeHeld();
		return;
	}
	else
		vy += dt * KOOPA_GRAVITY;
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
		if (state == KOOPA_STATE_WALKING)
		{
			if (vx < 0 && x < final_x) {
				x = final_x; vx = -vx;
			}

			if (vx > 0 && x > start_x) {
				x = start_x; vx = -vx;
			}
		}

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<CBrick*>(e->obj))
			{
				if (e->nx != 0)
				{
					this->x = x0 + min_tx * this->dx + e->nx * 0.1f;
					this->vx = -vx;
				}
				if (e->ny != 0)
				{
					this->vy = 0;
					this->y = y0 + min_ty * this->dy + e->ny * 0.1f;

				}
			}
			else if (dynamic_cast<CMario*>(e->obj))
			{
				/*CMario* mario = dynamic_cast<CMario*>(e->obj);
				if (e->nx != 0)
				{
					if()
					this->x = x0 + min_tx * this->dx + nx * 0.1f;
					this->vx = -vx;
				}
				if (e->ny != 0)
				{
					this->vy = 0;
					this->y = y0 + min_ty * this->dy + ny * 0.1f;

				}*/
			}

		}

	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}
void CKoopas::Render()
{
	
	int ani;
	//if (type == KOOPA_TYPE_RED_SMALL_TURTOISESHELL)
		ani = KOOPA_ANI_RED_SMALL_TURTOISESHELL_IDLE;

	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void CKoopas::SetState(int state)
{
	CGameObject::SetState(state);
	/*switch (state)
	{
	case KOOPAS_STATE_DIE:
		y += KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT_DIE + 1;
		vx = 0;
		vy = 0;
		break;
	case KOOPAS_STATE_WALKING:
		vx = KOOPAS_WALKING_SPEED;
	}*/

}
void CKoopas::BeHeld()
{
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (mario->IsHolding)
	{
		float l, t, r, b;
		mario->GetBoundingBox(l, t, r, b);
		if (mario->nx > 0)
			this->x = (r - 4);
		else
			this->x = l - 12;
		this->y = ((t + b) / 2 - 9);
		this->vx = this->vy = 0;
	}
	else
		IsBeingHeld = false;
}

void CKoopas::BeKicked(int mnx)
{
	this->nx = mnx;
	vx = nx * KOOPA_SPEED_TURTOISESHELL_X;
}

CKoopas::~CKoopas()
{

}