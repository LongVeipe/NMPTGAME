#include "Koopas.h"
#include "Koopa_Small.h"
#include "Brick.h"
#include "Mario.h"
#include "Game.h"
#include "PlayScence.h"
#include "Utils.h"

CKoopa_Small::CKoopa_Small(float _x, float _y, int _type) :CKoopas(_x, _y, _type)
{
	if (_type == KOOPA_SMALL_TYPE_RED_WALKING || _type == KOOPA_SMALL_TYPE_GREEN_WALKING)
		SetState(KOOPA_SMALL_STATE_WALKING_LEFT);
	else if (_type == KOOPA_SMALL_TYPE_RED_FLYING || _type == KOOPA_SMALL_TYPE_GREEN_FLYING)
	{
		SetState(KOOPA_SMALL_STATE_JUMPING_LEFT);
		leftWing = new CWing(WING_TYPE_LEFT);
		rightWing = new CWing(WING_TYPE_RIGHT);
	}
	else //type == turtoiseshell
		SetState(KOOPA_SMALL_STATE_IDLE);

	isEnable = true;
}

CKoopa_Small::~CKoopa_Small()
{
	delete rightWing;
	delete leftWing;
	rightWing = leftWing = nullptr;
}

void CKoopa_Small::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	if (type == KOOPA_SMALL_TYPE_RED_TURTOISESHELL || type == KOOPA_SMALL_TYPE_GREEN_TURTOISESHELL)
	{
		right = x + KOOPA_SMALL_TURTOISESHELL_BBOX_WIDTH;
		bottom = y + KOOPA_SMALL_TURTOISESHELL_BBOX_HEIGHT;
	}
	else
	{
		right = x + KOOPA_SMALL_BBOX_WIDTH;
		bottom = y + KOOPA_SMALL_BBOX_HEIGHT;
	}
}


void CKoopa_Small::Update_Wings()
{
	if (state == KOOPA_SMALL_STATE_JUMPING_LEFT)
		rightWing->SetPosition(x + 8, y - 1);
	else if(state == KOOPA_SMALL_STATE_JUMPING_RIGHT)
		leftWing->SetPosition(x, y - 1);
}

void CKoopa_Small::UpdateFlyingType()
{
	if (isTouchingGround)
		SetState(state);
}

void CKoopa_Small::Update_vy()
{
	vy += dt * KOOPA_GRAVITY;
	if (vy > KOOPA_MAX_FALL_SPEED)
		vy = KOOPA_MAX_FALL_SPEED;
}

bool CKoopa_Small::CalculateTurningAround(vector<LPGAMEOBJECT>* coObjects)
{
	//just use for walking type
	//
	if (!isTouchingGround)
		return false;
	float kl, kt, kr, kb;
	GetBoundingBox(kl, kt, kr, kb);
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT object = coObjects->at(i);
		if (!object->IsInCamera())
			continue;

		if (!dynamic_cast<CBrick*>(object))
			continue;

		//object is Break
		float ol, ot, or , ob;
		object->GetBoundingBox(ol, ot, or , ob);
		if (kb > ot - 2 && kb < ot)
		{
			if (state == KOOPA_SMALL_STATE_WALKING_LEFT)
			{
				if (kl > ol && kl < or )
					return false;
			}
			else if (state == KOOPA_SMALL_STATE_WALKING_RIGHT)
			{
				if (or > kr && ol < kr)
					return false;
			}
		}
		
	}
	return true;
}

void CKoopa_Small::TurnAround()
{
	if (state == KOOPA_SMALL_STATE_JUMPING_LEFT)
		SetState(KOOPA_SMALL_STATE_JUMPING_RIGHT);
	else if (state == KOOPA_SMALL_STATE_JUMPING_RIGHT)
		SetState(KOOPA_SMALL_STATE_JUMPING_LEFT);
	else if (state == KOOPA_SMALL_STATE_RUNNING_LEFT)
		SetState(KOOPA_SMALL_STATE_RUNNING_RIGHT);
	else if (state == KOOPA_SMALL_STATE_RUNNING_RIGHT)
		SetState(KOOPA_SMALL_STATE_RUNNING_LEFT);
	else if (state == KOOPA_SMALL_STATE_WALKING_LEFT)
		SetState(KOOPA_SMALL_STATE_WALKING_RIGHT);
	else if (state == KOOPA_SMALL_STATE_WALKING_RIGHT)
		SetState(KOOPA_SMALL_STATE_WALKING_LEFT);
}

void CKoopa_Small::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (!IsInCamera())
	{
		//if(!IsStart_xyInCamera())
		if (isEnable)
			Reset();
		else if(!isReadyToEnable)
		{
			isReadyToEnable = true;
		}
		return;
	}
	else
	{
		if (!isEnable)
		{
			if (isReadyToEnable)
			{
				isEnable = true;
				isReadyToEnable = false;
			}
			return;
		}

	}

	
	CGameObject::Update(dt, coObjects);
	if (IsBeingHeld)
	{
		BeHeld();
		return;
	}

	
	Update_vy();

	if ((type == KOOPA_SMALL_TYPE_RED_FLYING || type == KOOPA_SMALL_TYPE_GREEN_FLYING))
		UpdateFlyingType();

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
				if (brick->GetType() == BRICK_TYPE_BIG_BLOCK)
				{
					if (e->ny == -1)
					{
						this->vy = 0;
						this->y = y0 + min_ty * this->dy + e->ny * 0.1f;
					}
					else
					{
						x = x0 + dx;
						y = y0 + dy;
					}
				}
				else
				{
					if (e->nx != 0)
					{
						this->x = x0 + min_tx * this->dx + e->nx * 0.1f;
						TurnAround();
					}
					if (e->ny != 0)
					{
						this->vy = 0;
						this->y = y0 + min_ty * this->dy + e->ny * 0.1f;
					}
				}

				if (e->ny == -1 && isTouchingGround == false)
					isTouchingGround = true;
			}
			else if (dynamic_cast<CGoomba*>(e->obj))
			{

				CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
				if (type == KOOPA_SMALL_TYPE_RED_TURTOISESHELL || type == KOOPA_SMALL_TYPE_GREEN_TURTOISESHELL)
				{
					if (e->nx != 0)
					{
						if (this->state != KOOPA_SMALL_STATE_IDLE)
						{
							CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
							goomba->SetState(GOOMBA_STATE_DIE_X);
							goomba->vx = this->nx * GOOMBA_DIE_X_SPEED_X;
						}
					}
				}
			}
		}

	}
	//UpdaetState();
	if (CalculateTurningAround(coObjects) && type == KOOPA_SMALL_TYPE_RED_WALKING)
		TurnAround();

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	if (type == KOOPA_SMALL_TYPE_RED_FLYING || type == KOOPA_SMALL_TYPE_GREEN_FLYING)
		Update_Wings();

	//DebugOut(L"[INFO] isTouchingGround: %d \n", isTouchingGround);
}
void CKoopa_Small::Render()
{
	if (!isEnable)
		return;


	int ani = -1;
	if ( type == KOOPA_SMALL_TYPE_RED_WALKING)
	{
		if (state == KOOPA_SMALL_STATE_WALKING_LEFT)
			ani = KOOPA_SMALL_ANI_RED_WALKING_LEFT;
		else
			ani = KOOPA_SMALL_ANI_RED_WALKING_RIGHT;
	}
	else if (type == KOOPA_SMALL_TYPE_RED_FLYING)
	{
		if (state == KOOPA_SMALL_STATE_JUMPING_LEFT)
		{
			ani = KOOPA_SMALL_ANI_RED_WALKING_LEFT;
			rightWing->Render();
		}
		else
		{
			ani = KOOPA_SMALL_ANI_RED_WALKING_RIGHT;
			leftWing->Render();
		}
	}
	else if (type == KOOPA_SMALL_TYPE_GREEN_WALKING)
	{
		if ( state == KOOPA_SMALL_STATE_WALKING_LEFT)
			ani = KOOPA_SMALL_ANI_GREEN_WALKING_LEFT;
		else
			ani = KOOPA_SMALL_ANI_GREEN_WALKING_RIGHT;
	}
	else if (type == KOOPA_SMALL_TYPE_GREEN_FLYING)
	{
		if (state == KOOPA_SMALL_STATE_JUMPING_LEFT)
		{
			ani = KOOPA_SMALL_ANI_GREEN_WALKING_LEFT;
			rightWing->Render();
		}
		else
		{
			ani = KOOPA_SMALL_ANI_GREEN_WALKING_RIGHT;
			leftWing->Render();
		}
	}
	else if (type == KOOPA_SMALL_TYPE_RED_TURTOISESHELL)
	{
		if (state == KOOPA_SMALL_STATE_IDLE)
			ani = KOOPA_SMALL_ANI_RED_TURTOISESHELL_IDLE;
		else //if (state == KOOPA_SMALL_STATE_RUNNING_LEFT)
			ani = KOOPA_SMALL_ANI_RED_TURTOISESHELL_RUNNING;
		if (IsWaggling)
			ani = KOOPA_SMALL_ANI_RED_TURTOISESHELL_WAGGLE;
	}
	else if (type == KOOPA_SMALL_TYPE_GREEN_TURTOISESHELL)
	{
		if (state == KOOPA_SMALL_STATE_IDLE)
			ani = KOOPA_SMALL_ANI_GREEN_TURTOISESHELL_IDLE;
		else //if (state == KOOPA_SMALL_STATE_RUNNING_LEFT)
			ani = KOOPA_SMALL_ANI_GREEN_TURTOISESHELL_RUNNING;
		if (IsWaggling)
			ani = KOOPA_SMALL_ANI_GREEN_TURTOISESHELL_WAGGLE;
	}

	animation_set->at(ani)->Render(x, y);
}

void CKoopa_Small::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case KOOPA_SMALL_STATE_WALKING_RIGHT:
		vx = KOOPA_WALKING_SPEED;
		break;
	case KOOPA_SMALL_STATE_WALKING_LEFT:
		vx = -KOOPA_WALKING_SPEED;
		break;
	case KOOPA_SMALL_STATE_JUMPING_LEFT:
		vy = -KOOPA_JUMP_SPEED_Y;
		vx = -KOOPA_WALKING_SPEED;
		isTouchingGround = false;
		break;
	case KOOPA_SMALL_STATE_JUMPING_RIGHT:
		vy = -KOOPA_JUMP_SPEED_Y;
		vx = KOOPA_WALKING_SPEED;
		isTouchingGround = false;
		break;
	case KOOPA_SMALL_STATE_BE_KNOCKED_DOWN:
		vy = -KOOPA_BE_KNOCKED_DOWN_SPEED_Y;
		break;
	case KOOPA_SMALL_STATE_RUNNING_LEFT:
		vx = -KOOPA_SPEED_TURTOISESHELL_X;
		break;
	case KOOPA_SMALL_STATE_RUNNING_RIGHT:
		vx = KOOPA_SPEED_TURTOISESHELL_X;
		break;
	case KOOPA_SMALL_STATE_IDLE:
		vx = 0;
		break;
	}

}
void CKoopa_Small::BeHeld()
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
	{
		IsBeingHeld = false;
		if (mario->nx > 0)
			SetState(KOOPA_SMALL_STATE_RUNNING_RIGHT);
		else
			SetState(KOOPA_SMALL_STATE_RUNNING_LEFT);
	}
}

void CKoopa_Small::BeKicked(int mnx)
{
	this->nx = mnx;
	if (nx > 0)
		SetState(KOOPA_SMALL_STATE_RUNNING_RIGHT);
	else
		SetState(KOOPA_SMALL_STATE_RUNNING_LEFT);
}

void CKoopa_Small:: BeDamaged_Y()
{
	if (type == KOOPA_SMALL_TYPE_RED_FLYING || type == KOOPA_SMALL_TYPE_GREEN_FLYING)
	{
		delete leftWing;
		delete rightWing;
		leftWing = rightWing = nullptr;
		if (state == KOOPA_SMALL_STATE_JUMPING_LEFT)
			SetState(KOOPA_SMALL_STATE_WALKING_LEFT);
		else
			SetState(KOOPA_SMALL_STATE_WALKING_RIGHT);

		if (type == KOOPA_SMALL_TYPE_RED_FLYING)
			SetType(KOOPA_SMALL_TYPE_RED_WALKING);
		else
			SetType(KOOPA_SMALL_TYPE_GREEN_WALKING);
	}
	else if (type == KOOPA_SMALL_TYPE_GREEN_WALKING )
	{
		SetState(KOOPA_SMALL_STATE_IDLE);
		SetType(KOOPA_SMALL_TYPE_GREEN_TURTOISESHELL);
	}
	else if(type == KOOPA_SMALL_TYPE_RED_WALKING)
	{
		SetState(KOOPA_SMALL_STATE_IDLE);
		SetType(KOOPA_SMALL_TYPE_RED_TURTOISESHELL);
	}
	else // type == TURROISESHELL
	{
		if (state == KOOPA_SMALL_STATE_IDLE)
		{
			float kleft, ktop, kright, kbottom;
			this->GetBoundingBox(kleft, ktop, kright, kbottom);
			float mleft, mtop, mright, mbottom;
			((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer()->GetBoundingBox(mleft, mtop, mright, mbottom);
			if (mright < kright)
				this->SetState(KOOPA_SMALL_STATE_RUNNING_RIGHT);
			else
				this->SetState(KOOPA_SMALL_STATE_RUNNING_LEFT);
		}
		else
		{
			SetState(KOOPA_SMALL_STATE_IDLE);
		}
	}
	
}

void CKoopa_Small::CalculateBeSwingedTail()
{
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (mario->IsSwingTail)
	{
		float ml, mt, mr, mb;
		mario->GetBoundingBox(ml, mt, mr, mb);
		float kl, kt, kr, kb;
		this->GetBoundingBox(kl, kt, kr, kb);
		if (kb<mt || kt>mb || ml > kr || mr < kl)
			return;
		if ((kl<ml && kr>ml) || (kl < mr && mr < kr))
		{
			this->SetState(KOOPA_SMALL_STATE_BE_KNOCKED_DOWN);
			vx = mario->nx * KOOPA_BE_KNOCKED_DOWN_SPEED_X;
			
		}
	}
}

void CKoopa_Small::Reset()
{
	type = start_type;
	SetPosition(start_x, start_y);
	SetType(start_type);
	if (type == KOOPA_SMALL_TYPE_RED_WALKING || type == KOOPA_SMALL_TYPE_GREEN_WALKING)
		SetState(KOOPA_SMALL_STATE_WALKING_LEFT);
	else if (type == KOOPA_SMALL_TYPE_RED_FLYING || type == KOOPA_SMALL_TYPE_GREEN_FLYING)
	{
		SetState(KOOPA_SMALL_STATE_JUMPING_LEFT);
		if(!rightWing)
			delete rightWing;
		if(!rightWing)
			delete leftWing;
		leftWing = new CWing(WING_TYPE_LEFT);
		rightWing = new CWing(WING_TYPE_RIGHT);
	}
	else //type == turtoiseshell
		SetState(KOOPA_SMALL_STATE_IDLE);
	isEnable = false;
}