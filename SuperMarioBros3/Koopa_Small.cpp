#include "Koopas.h"
#include "Koopa_Small.h"
#include "Brick.h"
#include "Mario.h"
#include "Game.h"
#include "PlayScence.h"
#include "Utils.h"
#include "PointsEffect.h"
#include "RewardBox.h"
#include "IntroScene.h"

CKoopa_Small::CKoopa_Small(float _x, float _y, int _type) :CKoopas(_x, _y, _type)
{
	if (_type == KOOPA_SMALL_TYPE_RED_WALKING || _type == KOOPA_SMALL_TYPE_GREEN_WALKING)
		SetState(KOOPA_SMALL_STATE_WALKING_LEFT);
	else if ( _type == KOOPA_SMALL_TYPE_GREEN_FLYING)
	{
		SetState(KOOPA_SMALL_STATE_JUMPING_LEFT);
		leftWing = new CWing(WING_TYPE_LEFT);
		rightWing = new CWing(WING_TYPE_RIGHT);
	}
	else if (_type == KOOPA_SMALL_TYPE_RED_FLYING)
	{
		SetState(KOOPA_SMALL_STATE_FLYING_DOWN);
		leftWing = new CWing(WING_TYPE_LEFT);
		rightWing = new CWing(WING_TYPE_RIGHT);
		nx = -1;
	}
	else //type == turtoiseshell
		SetState(KOOPA_SMALL_STATE_IDLE);

	IsEnable = true;
}

CKoopa_Small::~CKoopa_Small()
{
	delete rightWing;
	delete leftWing;
	rightWing = leftWing = nullptr;
}

void CKoopa_Small::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (IsEnable)
	{
		if (state == KOOPA_SMALL_STATE_DIE)
		{
			left = top = right = bottom = 0;
			return;
		}
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
	else
		left = top = right = bottom = 0;
}

void CKoopa_Small::Update_Wings()
{
	if (!rightWing || !leftWing)
		return;
	if (state == KOOPA_SMALL_STATE_JUMPING_LEFT)
		rightWing->SetPosition(x + 8, y - 1);
	else if(state == KOOPA_SMALL_STATE_JUMPING_RIGHT)
		leftWing->SetPosition(x, y - 1);
	else if(nx == 1)
		leftWing->SetPosition(x, y - 1);
	else 
		rightWing->SetPosition(x + 8, y - 1);
}
void CKoopa_Small::UpdateGreenFlying()
{
	if (isTouchingGround)
		SetState(state);
}
void CKoopa_Small::UpdateRedFlying()
{
	if (state == KOOPA_SMALL_STATE_FLYING_DOWN)
	{
		vy -= KOOPA_RED_SMALL_FLYING_JET_SPEED;
		if (vy < 0)
			SetState(KOOPA_SMALL_STATE_FLYING_UP);
	}
	else if (state == KOOPA_SMALL_STATE_FLYING_UP )
	{
		vy += KOOPA_RED_SMALL_FLYING_JET_SPEED;
		if (vy > 0)
			SetState(KOOPA_SMALL_STATE_FLYING_DOWN);
	}
}
void CKoopa_Small::Update_vy()
{
	if (type == KOOPA_SMALL_TYPE_RED_FLYING)
		return;
	vy += dt * KOOPA_GRAVITY;
	if ( type == KOOPA_SMALL_TYPE_GREEN_FLYING)
	{
		if (vy > KOOPA_MAX_FALL_SPEED)
			vy = KOOPA_MAX_FALL_SPEED;
	}
	else
	{
		if (vy > 2.5f*KOOPA_MAX_FALL_SPEED)
			vy = 2.5f*KOOPA_MAX_FALL_SPEED;
	}
}
void CKoopa_Small::UpdateFlag()
{
	if (type == KOOPA_SMALL_TYPE_GREEN_TURTOISESHELL || type == KOOPA_SMALL_TYPE_RED_TURTOISESHELL)
	{
		if (state == KOOPA_SMALL_STATE_RUNNING_LEFT || state == KOOPA_SMALL_STATE_RUNNING_RIGHT)
		{
			isTurtoiseshell_start = (DWORD)GetTickCount64();
		}
		else if (GetTickCount64() - isTurtoiseshell_start > KOOPA_SMALL_IS_TURTOISESHELL_TIME - KOOPA_SMALL_WAGGLE_TIME && !IsWaggling)
		{
			IsWaggling = true;
		}
		else if (GetTickCount64() - isTurtoiseshell_start > KOOPA_SMALL_IS_TURTOISESHELL_TIME && IsWaggling)
		{
			IsBeingKnockedDown = false;
			IsWaggling = false;
			y = y + KOOPA_SMALL_TURTOISESHELL_BBOX_HEIGHT - KOOPA_SMALL_BBOX_HEIGHT;
			SetState(KOOPA_SMALL_STATE_WALKING_LEFT);
			if (type == KOOPA_SMALL_TYPE_GREEN_TURTOISESHELL)
				SetType(KOOPA_SMALL_TYPE_GREEN_WALKING);
			else
				SetType(KOOPA_SMALL_TYPE_RED_WALKING);
			IsBeingHeld = false;
			if(holder != nullptr)
				if (holder->IsHolding)
				{
					holder->IsHolding = false;
					holder->BeDamaged();
				}
		}
	}
}

void CKoopa_Small::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CScene* s = CGame::GetInstance()->GetCurrentScene();
	if (dynamic_cast<CPlayScene*>(s))
	{
		if (!IsInCamera())
		{
			if (IsEnable)
				Reset();
			else if (!isReadyToEnable)
			{
				isReadyToEnable = true;
			}
			return;
		}
		else
		{
			if (!IsEnable)
			{
				if (isReadyToEnable)
				{
					IsEnable = true;
					isReadyToEnable = false;
				}
				return;
			}

		}
	}
	else if (dynamic_cast<CIntroScene*>(s))
	{
		if (!IsEnable)
			return;
	}
	
	CGameObject::Update(dt, coObjects);

	UpdateFlag();
	if (IsBeingHeld)
	{
		BeHeld();
		return;
	}
	
	Update_vy();
	
	if ( type == KOOPA_SMALL_TYPE_GREEN_FLYING)
		UpdateGreenFlying();
	else if(type == KOOPA_SMALL_TYPE_RED_FLYING)
		UpdateRedFlying();
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
						if (state == KOOPA_SMALL_STATE_BE_KNOCKED_DOWN)
							SetState(KOOPA_SMALL_STATE_IDLE);
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
						if (state == KOOPA_SMALL_STATE_BE_KNOCKED_DOWN)
							SetState(KOOPA_SMALL_STATE_IDLE);
					}
				}

				if (e->ny == -1 && isTouchingGround == false)
				{
					isTouchingGround = true;
					if (state == KOOPA_SMALL_STATE_IDLE)
						vx = 0;
				}
			}
			else if (dynamic_cast<CRewardBox*>(e->obj))
			{
				CRewardBox* rBox = dynamic_cast<CRewardBox*>(e->obj);
				if (e->nx != 0)
				{
					this->x = x0 + min_tx * this->dx + e->nx * 0.1f;

					TurnAround();
					if (type == KOOPA_SMALL_TYPE_GREEN_TURTOISESHELL || type == KOOPA_SMALL_TYPE_RED_TURTOISESHELL)
					{
						rBox->BeAttacked();
					}
				}
				if (e->ny != 0)
				{
					this->vy = 0;
					this->y = y0 + min_ty * this->dy + e->ny * 0.1f;
					if (state == KOOPA_SMALL_STATE_BE_KNOCKED_DOWN)
						SetState(KOOPA_SMALL_STATE_IDLE);
					if (e->ny == -1 && isTouchingGround == false)
						isTouchingGround = true;
				}
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
							goomba->BeDamaged_X(this);
						}
					}
				}
			}
			else if (dynamic_cast<CKoopa_Small*>(e->obj))
			{
				CKoopa_Small* koopa = dynamic_cast<CKoopa_Small*>(e->obj);
				if (koopa->type == KOOPA_SMALL_TYPE_GREEN_TURTOISESHELL || koopa->type == KOOPA_SMALL_TYPE_RED_TURTOISESHELL)
				{
					
					if (koopa->IsBeingHeld)
					{
						koopa->SetState(KOOPA_SMALL_STATE_DIE);
						this->SetState(KOOPA_SMALL_STATE_DIE);
						koopa->IsBeingHeld = false;
						if (koopa->holder)
							koopa->holder->IsHolding = false;
					}
					else
					{
						if (koopa->state == KOOPA_SMALL_STATE_IDLE)
							this->TurnAround();
						else
						{
							this->SetState(KOOPA_SMALL_STATE_DIE);
							this->vx = koopa->nx * KOOPA_BE_KNOCKED_DOWN_SPEED_X;
						}
					}
				}
			}
		}

	}

	if (CalculateTurningAround(coObjects))
		TurnAround();
	CalculateBeSwingedTail();
	CalculateBeAtackedByBox(coObjects);

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	if (type == KOOPA_SMALL_TYPE_RED_FLYING || type == KOOPA_SMALL_TYPE_GREEN_FLYING)
		Update_Wings();

	//DebugOut(L"[INFO] stateUpdate: %d \n", state);

}

void CKoopa_Small::Render()
{
	if (!IsEnable)
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
		if (nx == -1)
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
		if (state == KOOPA_SMALL_STATE_DIE)
			ani = KOOPA_SMALL_ANI_RED_BE_KNOCKED_DOWN_IDLE;
		else if (!IsBeingKnockedDown)
		{
			if (state == KOOPA_SMALL_STATE_IDLE)
				ani = KOOPA_SMALL_ANI_RED_TURTOISESHELL_IDLE;
			else if (state == KOOPA_SMALL_STATE_RUNNING_LEFT || state == KOOPA_SMALL_STATE_RUNNING_RIGHT)
				ani = KOOPA_SMALL_ANI_RED_TURTOISESHELL_RUNNING;

			if (IsWaggling)
				ani = KOOPA_SMALL_ANI_RED_TURTOISESHELL_WAGGLE;
		}
		else
		{
			if (state == KOOPA_SMALL_STATE_IDLE || state == KOOPA_SMALL_STATE_BE_KNOCKED_DOWN)
				ani = KOOPA_SMALL_ANI_RED_BE_KNOCKED_DOWN_IDLE;
			else if (state == KOOPA_SMALL_STATE_RUNNING_LEFT || state == KOOPA_SMALL_STATE_RUNNING_RIGHT)
				ani = KOOPA_SMALL_ANI_RED_BE_KNOCKED_DOWN_RUNNING;

			if (IsWaggling)
				ani = KOOPA_SMALL_ANI_RED_BE_KNOCKED_DOWN_WAGGLE;
		}

	}
	else if (type == KOOPA_SMALL_TYPE_GREEN_TURTOISESHELL)
	{
		if (state == KOOPA_SMALL_STATE_DIE)
		{
			ani = KOOPA_SMALL_ANI_GREEN_BE_KNOCKED_DOWN_IDLE;
		}
		else if (!IsBeingKnockedDown)
		{
			if (state == KOOPA_SMALL_STATE_IDLE)
				ani = KOOPA_SMALL_ANI_GREEN_TURTOISESHELL_IDLE;
			else if (state == KOOPA_SMALL_STATE_RUNNING_LEFT || state == KOOPA_SMALL_STATE_RUNNING_RIGHT)
				ani = KOOPA_SMALL_ANI_GREEN_TURTOISESHELL_RUNNING;

			if (IsWaggling)
				ani = KOOPA_SMALL_ANI_GREEN_TURTOISESHELL_WAGGLE;
		}
		else
		{
			if (state == KOOPA_SMALL_STATE_IDLE || state == KOOPA_SMALL_STATE_BE_KNOCKED_DOWN )
				ani = KOOPA_SMALL_ANI_GREEN_BE_KNOCKED_DOWN_IDLE;
			else if (state == KOOPA_SMALL_STATE_RUNNING_LEFT || state == KOOPA_SMALL_STATE_RUNNING_RIGHT)
				ani = KOOPA_SMALL_ANI_GREEN_BE_KNOCKED_DOWN_RUNNING;

			if (IsWaggling)
				ani = KOOPA_SMALL_ANI_GREEN_BE_KNOCKED_DOWN_WAGGLE;
		}
	}

	animation_set->at(ani)->Render(x, y);
}

void CKoopa_Small::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case KOOPA_SMALL_STATE_WALKING_RIGHT:
		nx = 1;
		vx = KOOPA_WALKING_SPEED;
		break;
	case KOOPA_SMALL_STATE_WALKING_LEFT:
		nx = -1;
		vx = -KOOPA_WALKING_SPEED;
		break;
	case KOOPA_SMALL_STATE_JUMPING_LEFT:
		nx = -1;
		vy = -KOOPA_JUMP_SPEED_Y;
		vx = -KOOPA_WALKING_SPEED;
		isTouchingGround = false;
		break;
	case KOOPA_SMALL_STATE_JUMPING_RIGHT:
		nx = 1;
		vy = -KOOPA_JUMP_SPEED_Y;
		vx = KOOPA_WALKING_SPEED;
		isTouchingGround = false;
		break;
	case KOOPA_SMALL_STATE_RUNNING_LEFT:
		nx = -1;
		vx = -KOOPA_SPEED_TURTOISESHELL_X;
		break;
	case KOOPA_SMALL_STATE_RUNNING_RIGHT:
		nx = 1;
		vx = KOOPA_SPEED_TURTOISESHELL_X;
		break;
	case KOOPA_SMALL_STATE_BE_KNOCKED_DOWN:
		IsBeingKnockedDown = true;
		vy = -KOOPA_BE_KNOCKED_DOWN_SPEED_Y;
		if (IsOnTheLeftOfMario())
			vx = -KOOPA_BE_KNOCKED_DOWN_SPEED_X;
		else
			vx = KOOPA_BE_KNOCKED_DOWN_SPEED_X;
		break;
	case KOOPA_SMALL_STATE_FLYING_DOWN:
		vy = KOOPA_RED_SMALL_FLYING_MAIN_SPEED;
		break;
	case KOOPA_SMALL_STATE_FLYING_UP:
		vy = -KOOPA_RED_SMALL_FLYING_MAIN_SPEED;
		break;
	case KOOPA_SMALL_STATE_IDLE:
		vx = 0;
		break;
	case KOOPA_SMALL_STATE_DIE:
		vy = -KOOPA_BE_KNOCKED_DOWN_SPEED_Y;
		switch (type)
		{
		case KOOPA_SMALL_TYPE_RED_FLYING:
		case KOOPA_SMALL_TYPE_RED_WALKING:
			SetType(KOOPA_SMALL_TYPE_RED_TURTOISESHELL);
			break;
		case KOOPA_SMALL_TYPE_GREEN_WALKING:
		case KOOPA_SMALL_TYPE_GREEN_FLYING:
			SetType(KOOPA_SMALL_TYPE_GREEN_TURTOISESHELL);
			break;
		}
		break;
	}

}

void CKoopa_Small::SetType(int _type)
{
	CEnemy::SetType(_type);
	if (_type == KOOPA_SMALL_TYPE_RED_TURTOISESHELL || _type == KOOPA_SMALL_TYPE_GREEN_TURTOISESHELL)
		isTurtoiseshell_start =(DWORD) GetTickCount64();
}

void CKoopa_Small::BeHeld()
{
	if (holder->IsHolding)
	{
		float l, t, r, b;
		holder->GetBoundingBox(l, t, r, b);
		if (holder->nx > 0)
			this->x = (r - 4);
		else
			this->x = l - 12;
		this->y = ((t + b) / 2 - 9);
		this->vx = holder->vx;
		this->vy = holder->vy;
	}
	else
	{
		IsBeingHeld = false;
		if (holder->nx > 0)
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
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
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
		mario->UpPoints(POINTS_100);
		CPointsEffect* pe = new CPointsEffect(x, y, POINTS_100);
		CPointsEffects::GetInstance()->Add(pe);
	}
	else if(type == KOOPA_SMALL_TYPE_RED_WALKING)
	{
		SetState(KOOPA_SMALL_STATE_IDLE);
		SetType(KOOPA_SMALL_TYPE_RED_TURTOISESHELL);
		mario->UpPoints(POINTS_100);
		CPointsEffect* pe = new CPointsEffect(x, y, POINTS_100);
		CPointsEffects::GetInstance()->Add(pe);
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
			mario->UpPoints(POINTS_200);
			CPointsEffect* pe = new CPointsEffect(x, y, POINTS_200);
			CPointsEffects::GetInstance()->Add(pe);
		}
		else
		{
			SetState(KOOPA_SMALL_STATE_IDLE);
			mario->UpPoints(POINTS_100);
			CPointsEffect* pe = new CPointsEffect(x, y, POINTS_100);
			CPointsEffects::GetInstance()->Add(pe);
		}
	}
	
}

void CKoopa_Small::BeSwingTail(CMario* mario)
{
	SetState(KOOPA_SMALL_STATE_BE_KNOCKED_DOWN);
	if (IsOnTheLeftOfMario())
		vx = -KOOPA_BE_KNOCKED_DOWN_SPEED_X;
	else 
		vx = KOOPA_BE_KNOCKED_DOWN_SPEED_X;

	//setType
	switch (type)
	{
	case KOOPA_SMALL_TYPE_RED_FLYING:
	case KOOPA_SMALL_TYPE_RED_WALKING:
		SetType(KOOPA_SMALL_TYPE_RED_TURTOISESHELL);
		break;
	case KOOPA_SMALL_TYPE_GREEN_FLYING:
	case KOOPA_SMALL_TYPE_GREEN_WALKING:
		SetType(KOOPA_SMALL_TYPE_GREEN_TURTOISESHELL);
		break;
	}

	//Add effect
	mario->UpPoints(POINTS_100);
	CPointsEffect* pe = new CPointsEffect(x, y, POINTS_100);
	CPointsEffects::GetInstance()->Add(pe);
}
void CKoopa_Small::CalculateBeSwingedTail()
{
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (mario->IsSwingTail)
	{
		float ml, mt, mr, mb;
		mario->GetBoundingBox(ml, mt, mr, mb);
		float gl, gt, gr, gb;
		this->GetBoundingBox(gl, gt, gr, gb);
		ml -= MARIO_RACCOON_TAIL_BBOX_WIDTH;
		mr += MARIO_RACCOON_TAIL_BBOX_WIDTH;
		if (gb<mt || gt>mb || ml > gr || mr < gl)
			return;
		if ((gl<ml && gr>ml) || (gl < mr && mr < gr))
		{
			BeSwingTail(mario);
		}
	}
}
void CKoopa_Small::CalculateBeAtackedByBox(vector<LPGAMEOBJECT>* coObjects)
{
	for (unsigned int i = 0; i < coObjects->size(); i++)
	{
		if (dynamic_cast<CRewardBox*>(coObjects->at(i)))
		{
			CRewardBox* box = dynamic_cast<CRewardBox*>(coObjects->at(i));
			if (box->GetState() == REWARD_BOX_STATE_JUMPING)
			{
				float bl, bt, br, bb;
				box->GetBoundingBox(bl, bt, br, bb);
				float kl, kt, kr, kb;
				this->GetBoundingBox(kl, kt, kr, kb);
				if (kb<bt || kt>bb || bl > kr || br < kl)
					return;
				if ((kl<bl && kr>bl) || (kl < br && br < kr))
				{
					SetState(KOOPA_SMALL_STATE_BE_KNOCKED_DOWN);
					if (IsOnTheLeftOfMario())
						vx = -KOOPA_BE_KNOCKED_DOWN_SPEED_X;
					else
						vx = KOOPA_BE_KNOCKED_DOWN_SPEED_X;

					//setType
					switch (type)
					{
					case KOOPA_SMALL_TYPE_RED_FLYING:
					case KOOPA_SMALL_TYPE_RED_WALKING:
						SetType(KOOPA_SMALL_TYPE_RED_TURTOISESHELL);
						break;
					case KOOPA_SMALL_TYPE_GREEN_FLYING:
					case KOOPA_SMALL_TYPE_GREEN_WALKING:
						SetType(KOOPA_SMALL_TYPE_GREEN_TURTOISESHELL);
						break;
					}
					return;
				}
			}
		}
	}
}

bool CKoopa_Small::CalculateTurningAround(vector<LPGAMEOBJECT>* coObjects)
{
	//just use for red walking type
	//
	if (type != KOOPA_SMALL_TYPE_RED_WALKING)
		return false;
	if (!isTouchingGround)
		return false;
	float kl, kt, kr, kb;
	GetBoundingBox(kl, kt, kr, kb);
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT object = coObjects->at(i);
		if (!object->IsInCamera())
			continue;

		if (!dynamic_cast<CBrick*>(object) && !dynamic_cast<CRewardBox*>(object))
			continue;

		//object is Break or RBox
		float ol, ot, or , ob;
		object->GetBoundingBox(ol, ot, or , ob);
		/*if (object->x == 2096 && object->y == 384)
			DebugOut(L"kb: %f, ot: %f \n", kb, ot);*/
		if (kb > ot - 2 && kb < ot)
		{
			if (state == KOOPA_SMALL_STATE_WALKING_LEFT)
			{
				if ((kl + 4 > ol && kl + 8 < or ) || (kl + 4 <= or && kl + 8 >= or))
					return false;
			}
			else if (state == KOOPA_SMALL_STATE_WALKING_RIGHT)
			{
				if ((kr - 8 > ol && kr - 4 < or ) || (kr - 8 <= ol && kr - 4 >= ol))
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

void CKoopa_Small::Reset()
{
	if (state == KOOPA_SMALL_STATE_DIE)
		return;
	type = start_type;
	SetPosition(start_x, start_y);
	SetType(start_type);
	if (type == KOOPA_SMALL_TYPE_RED_WALKING || type == KOOPA_SMALL_TYPE_GREEN_WALKING)
		SetState(KOOPA_SMALL_STATE_WALKING_LEFT);
	else if ( type == KOOPA_SMALL_TYPE_GREEN_FLYING)
	{
		SetState(KOOPA_SMALL_STATE_JUMPING_LEFT);
		if(!rightWing)
			delete rightWing;
		if(!rightWing)
			delete leftWing;
		leftWing = new CWing(WING_TYPE_LEFT);
		rightWing = new CWing(WING_TYPE_RIGHT);
	}
	else if (type == KOOPA_SMALL_TYPE_RED_FLYING)
	{
		SetState(KOOPA_SMALL_STATE_FLYING_DOWN);
		leftWing = new CWing(WING_TYPE_LEFT);
		rightWing = new CWing(WING_TYPE_RIGHT);
		nx = -1;
	}
	else //type == turtoiseshell
		SetState(KOOPA_SMALL_STATE_IDLE);
	IsEnable = false;
}

bool CKoopa_Small::IsOnTheLeftOfMario()
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