#include "Goomba.h"
#include "Brick.h"
#include "Mario.h"
#include "PlayScence.h"
#include "Utils.h"
CGoomba::CGoomba(float start_x, float final_x, int _type):CGameObject()
{
	type = _type;
	SetState(GOOMBA_STATE_WALKING);
	this->PARA_jumpStack = 0;
	loop_start == GetTickCount64();
}
CGoomba::~CGoomba()
{

}
void CGoomba::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (this->state == GOOMBA_STATE_DIE_X || this->state == GOOMBA_STATE_DIE_Y)
			left = top = right = bottom = 0;
	else
	{
		if (type == GOOMBA_TYPE_SMALL)
		{
			left = x;
			top = y;
			right = x + GOOMBA_BBOX_WIDTH;
			bottom = y + GOOMBA_BBOX_HEIGHT;
		}
		else
		{
			left = x + GOOMBA_BBOX_WING_WIDTH;
			right = left + GOOMBA_BBOX_WIDTH;
			/*if (IsTouchingGround)
				top = y + GOOMBA_BBOX*/
			top = y;
			bottom = y + GOOMBA_BBOX_HEIGHT + GOOMBA_BBOX_FOLDING_WING_HEIGHT;
		}
	}
}

void CGoomba::Calculate_vy()
{
	if (state != GOOMBA_STATE_DIE_Y)
	{
		vy += dt * GOOMBA_GRAVITY;
	}
	if (vy > GOOMBA_MAX_FALL_SPEED)
		vy = GOOMBA_MAX_FALL_SPEED;
}
void CGoomba::Calculate_vx()
{

}
void CGoomba::Update_Para()
{
	//update vx to attack on mario 
	if (IsTouchingGround)
	{
		if ((GetTickCount64() - loop_start) > GOOMBA_TIME_PARA_OPERATION_LOOP && PARA_jumpStack == 0)
		{
			CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
			float ml, mt, mr, mb;
			mario->GetBoundingBox(ml, mt, mr, mb);
			float gl, gt, gr, gb;
			this->GetBoundingBox(gl, gt, gr, gb);
			if (mr < gl)
				vx = -GOOMBA_WALKING_SPEED;
			else if(ml>gr)
				vx = GOOMBA_WALKING_SPEED;
		}
	}
	//update jumping
	if (IsTouchingGround)
	{
		if ((GetTickCount64() - loop_start) > GOOMBA_TIME_PARA_OPERATION_LOOP && PARA_jumpStack == 0 )
		{
			SetState(GOOMBA_STATE_JUMPING);
			loop_start = GetTickCount64();
			PARA_jumpStack++;
		}
		else if (PARA_jumpStack == GOOMBA_PARA_MAX_JUMP_STACK)
		{
			SetState(GOOMBA_STATE_FLYING);
			PARA_jumpStack = 0;
		}
		else if(PARA_jumpStack > 0 && PARA_jumpStack<GOOMBA_PARA_MAX_JUMP_STACK)
		{
			SetState(GOOMBA_STATE_JUMPING);
			PARA_jumpStack++;
		}
	}

}
void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	if (!IsInCamera())
		return;
	CGameObject::Update(dt, coObjects);

	Calculate_vy();
	if (type == GOOMBA_TYPE_PARA)
		Update_Para();
	
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
				if (nx != 0)
				{
					this->x = x0 + min_tx * this->dx + nx * 0.1f;
					this->vx = -vx;
				}
				if (ny != 0)
				{
					IsTouchingGround = true;
					state = GOOMBA_STATE_WALKING;
					this->vy = 0;
					this->y = y0 + min_ty * this->dy + ny * 0.1f;
					
				}
			}
			else if (dynamic_cast<CMario*>(e->obj))
			{
				/*if (nx != 0)
				{
					this->x = x0 + min_tx * this->dx + nx * 0.1f;
					this->vx = -vx;
				}*/
			}

		}

	}
	CalculateBeSwingedTail();
	 //clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CGoomba::Render()
{
	if (!IsInCamera())
		return;


	int ani = -1;
	if (this->state == GOOMBA_STATE_DIE_Y)
	{
		ani = GOOMBA_ANI_DIE_Y;
		if (DeadTime != 0 && (GetTickCount64() - this->DeadTime) >= GOOMBA_TIME_TO_STOP_RENDERING)
			return;
	}
	else if (state == GOOMBA_STATE_DIE_X)
		ani = GOOMBA_ANI_DIE_X;
	else if (type == GOOMBA_TYPE_SMALL)
		ani = GOOMBA_ANI_SMALL_WALKING;
	else if (type == GOOMBA_TYPE_PARA)
	{
		if (state == GOOMBA_STATE_WALKING)
			ani = GOOMBA_ANI_PARA_WALKING;
		else if (state == GOOMBA_STATE_JUMPING)
			ani = GOOMBA_ANI_PARA_JUMPING;
		else if (state == GOOMBA_STATE_FLYING)
			ani = GOOMBA_ANI_PARA_FLYING;
	}
	

	animation_set->at(ani)->Render(x, y);


	//RenderBoundingBox();
}

void CGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
		case GOOMBA_STATE_DIE_Y:
			y += GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE + 1;
			vx = 0;
			vy = 0;
			break;
		case GOOMBA_STATE_DIE_X:
			vy = -GOOMBA_DIE_X_SPEED_Y;
			vx = 0;
			break;
		case GOOMBA_STATE_WALKING: 
			vx = -GOOMBA_WALKING_SPEED;
			//vx = GOOMBA_WALKING_SPEED;
			break;
		case GOOMBA_STATE_JUMPING:
			IsTouchingGround = false;
			vy = -GOOMBA_JUMP_SPEED_Y;
			break;
		case GOOMBA_STATE_FLYING:
			IsTouchingGround = false;
			vy = -GOOMBA_FLY_SPEED_Y;
			break;
	}
}

void CGoomba::SetDeadTime()
{
	this->DeadTime = GetTickCount();
}

void CGoomba::CalculateBeSwingedTail()
{
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (mario->IsSwingTail)
	{
		float ml, mt, mr, mb;
		mario->GetBoundingBox(ml, mt, mr, mb);
		float gl, gt, gr, gb;
		this->GetBoundingBox(gl,gt,gr,gb);
		if (gb<mt || gt>mb || ml > gr || mr < gl)
			return;
		if ((gl<ml && gr>ml) ||(gl<mr &&mr<gr))
		{
			this->SetState(GOOMBA_STATE_DIE_X);
			vx = mario->nx * GOOMBA_DIE_X_SPEED_X;
		}
	}
}

void CGoomba::BeDamaged_Y()
{
	if (type > GOOMBA_TYPE_SMALL)
		type = GOOMBA_TYPE_SMALL;
	else
	{
		SetState(GOOMBA_STATE_DIE_Y);
		SetDeadTime();
	}
}

