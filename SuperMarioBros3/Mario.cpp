#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "Mario.h"
#include "Game.h"

#include "Goomba.h"
#include "Portal.h"
#include "Brick.h"
#include "SuperLeaf.h"
#include "Koopas.h"

CMario::CMario(float x, float y) : CGameObject()
{
	level = MARIO_LEVEL_BIG;
	untouchable = 0;
	SetState(MARIO_STATE_IDLE);

	start_x = x; 
	start_y = y; 
	this->x = x; 
	this->y = y;
	jumpStack = 0;
	imminentStack = 0;
	IsReadyJump = true;
	IsTouchingGround = true;
	IsJumping = false;
	IsFalling = false;
	IsFlying = false;

	IsWalkingRight = false;
	IsWalkingLeft = false;

	IsReadyHolding = false;
	IsHolding = false;
}

void CMario::Calculate_vx(DWORD _dt)
{
	vx += ax * _dt;
	//state == walking
	if (abs(vx) > MARIO_WALKING_SPEED_MAX)
		vx = nx * MARIO_WALKING_SPEED_MAX;
	//state == idle
	if (state == MARIO_STATE_IDLE)
	{
		if (nx > 0)
		{
			if (vx < 0)
				vx = 0;
		}
		else
			if (vx > 0)
				vx = 0;
	}

}
void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down

	vy += MARIO_GRAVITY*dt;
	Calculate_vx(dt);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state!=MARIO_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	if ( GetTickCount() - untouchable_start > MARIO_UNTOUCHABLE_TIME) 
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	// No collision occured, proceed normally
	if (coEvents.size()==0)
	{
		if (x + dx < 0)
			x = 0;
		else
			x += dx; 


		if (y + dy < -30)
			y = -30;
		else
			y += dy;
		//IsTouchingGround = false;
	}
	else
	{
 		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0; 
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		//if (rdx != 0 && rdx!=dx)
		//	x += nx*abs(rdx); 
		// block every object first!
		

		//
		// Collision logic with other objects
		//
		float x0 = x, y0 = y;
		x = x0 + dx;
		y = y0 + dy;
	
		float oleft, otop, oright, obottom;

		
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			e->obj->GetBoundingBox(oleft, otop, oright, obottom);
			if (dynamic_cast<CBrick*>(e->obj))
			{
				CBrick* brick = dynamic_cast<CBrick*>(e->obj);
				switch (brick->GetType())
				{
				case BRICK_TYPE_NORMAL:
					BasicCollision(min_tx, min_ty, e->nx, e->ny, x0, y0, oleft, otop, oright, obottom);
					break;
				case BRICK_TYPE_BIG_BLOCK:
				{
					if (e->ny == -1)
					{
						BasicCollision(min_tx, min_ty, e->nx, e->ny, x0, y0, oleft, otop, oright, obottom);
					}
					else
					{
						x = x0 + dx;
						y = y0 + dy;
					}
					break;
				}
				case BRICK_TYPE_QUESTION:
					
					BasicCollision(min_tx, min_ty, e->nx, e->ny, x0, y0, oleft, otop, oright, obottom);
					if (brick->GetDetailType() == BRICK_DETAIL_TYPE_QUESTION_INTACT  )
					{
						if (e->ny == 1)
							//this->ny = 1;
						{
							if (dynamic_cast<CCoin*>(brick->GetReward()))
							{
								CCoin* coin = (CCoin*)brick->GetReward();
								coin->SetState(COIN_STATE_SHOW_JUMP);
							}
							else if (dynamic_cast<CSuperLeaf*>(brick->GetReward()))
							{
								CSuperLeaf* leaf = (CSuperLeaf*)brick->GetReward();
								leaf->SetState(SUPER_LEAF_STATE_JUMP);
								
							}
							brick->SetState(BRICK_STATE_JUMP);
							brick->SetDetailType(BRICK_DETAIL_TYPE_QUESTION_EMPTY);
							
						}
						else if (e->nx == 1 || e->nx == -1)
						{

						}
					}

					break;
				}
				
			}
			else if(dynamic_cast<CGoomba*>(e->obj)) // if e->obj is Goomba 
			{
				
				CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

				// jump on top >> kill Goomba and deflect a bit 
				if (e->ny < 0)
				{
					BasicCollision(min_tx, min_ty, e->nx, e->ny, x0, y0, oleft, otop, oright, obottom);
					if (goomba->GetState() != GOOMBA_STATE_DIE)
					{
						goomba->SetState(GOOMBA_STATE_DIE);
						goomba->SetDeadTime();
						vy = -MARIO_JUMP_DEFLECT_SPEED;
						ny = -1;
					}
				}
				else if (e->nx != 0)
				{
					if (untouchable == 0)
					{
						if (goomba->GetState() != GOOMBA_STATE_DIE)
						{
							if (level > MARIO_LEVEL_SMALL)
							{
								level = MARIO_LEVEL_SMALL;
								StartUntouchable();
							}
							else
								SetState(MARIO_STATE_DIE);
						}
					}
				}
			} // if Goomba
			else if (dynamic_cast<CCoin*>(e->obj)) // if e->obj is Coin
			{
				CCoin* coin = dynamic_cast<CCoin*>(e->obj);
				coin->SetState(COIN_STATE_HIDDEN);
			}
			else if (dynamic_cast<CKoopas*>(e->obj))
			{
				CKoopas* koopa = dynamic_cast<CKoopas*>(e->obj);
				switch (koopa->GetType())
				{
				case KOOPA_TYPE_RED_SMALL_TURTOISESHELL:
					if (e->nx != 0)
					{
						if (IsReadyHolding == true)
						{
							IsHolding = true;
							koopa->IsBeingHeld = true;
							IsReadyHolding = false;

						}
						else
						{
							
						}

					}
					if (e->ny != 0)
					{

					}
					break;
				}
			}
			else if (dynamic_cast<CPortal*>(e->obj))
			{
				BasicCollision(min_tx, min_ty, e->nx, e->ny, x0, y0, oleft, otop, oright, obottom);

				if (e->ny == -1)
				{
					if (this->state == MARIO_STATE_BEND_DOWN)
					{
						CPortal* p = dynamic_cast<CPortal*>(e->obj);
						CGame::GetInstance()->SwitchScene(p->GetSceneId());
					}
				}
			}

		}
		
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) 
		delete coEvents[i];
}

void CMario::BasicCollision(float min_tx, float min_ty, float _nx, float _ny, float x0, float y0, float oleft, float otop, float oright, float obottom)
{
	float mleft, mtop, mright, mbottom;
	GetBoundingBox(mleft, mtop, mright, mbottom);
	if (_nx !=0 )
	{
		if (ceil(mbottom) -1  != otop)
		{
			this->vx = 0;
			this->x = x0 + min_tx * this->dx + _nx * 0.1f;
		}
	}
	if (_ny != 0)
	{
		this->vy = 0;
		this->y = y0 + min_ty * this->dy + _ny * 0.1f;
		if (_ny == -1)
		{
			this->ny = 0;
			IsTouchingGround = true;
			//this->SetState(MARIO_STATE_IDLE);
		}
		else if (_ny == 1)
		{
			IsReadyJump = false;
		}
	}
}

void CMario::Render()
{
	int ani = -1;
	if (state == MARIO_STATE_DIE)
		ani = MARIO_ANI_DIE;
	else if(level == MARIO_LEVEL_BIG)
	{
		if (state == MARIO_STATE_WALKING_LEFT)
			if (vx >= 0)
				ani = MARIO_ANI_BIG_SKIDDING_LEFT;
			else
				ani = MARIO_ANI_BIG_WALKING_LEFT;
		else if (state == MARIO_STATE_WALKING_RIGHT)
			if (vx <= 0)
				ani = MARIO_ANI_BIG_SKIDDING_RIGHT;
			else
				ani = MARIO_ANI_BIG_WALKING_RIGHT;
		else if (state == MARIO_STATE_JUMP)
		{
			if (nx > 0)
				ani = MARIO_ANI_BIG_JUMPING_RIGHT;
			else if (nx < 0)
				ani = MARIO_ANI_BIG_JUMPING_LEFT;
		}
		else //if (state == MARIO_STATE_IDLE)
		{
			if (vx > 0)
				ani = MARIO_ANI_BIG_WALKING_RIGHT;
			else if (vx < 0)
				ani = MARIO_ANI_BIG_WALKING_LEFT;
			else
				if (nx > 0)
					ani = MARIO_ANI_BIG_IDLE_RIGHT;
				else
					ani = MARIO_ANI_BIG_IDLE_LEFT;
		}
		if (IsHolding)
		{
			if (vx > 0)
				ani = MARIO_ANI_BIG_HOLDING_RIGHT;
			else if (vx < 0)
				ani = MARIO_ANI_BIG_HOLDING_LEFT;
			else
				if (nx > 0)
					ani = MARIO_ANI_BIG_HOLDING_IDLE_RIGHT;
				else
					ani = MARIO_ANI_BIG_HOLDING_IDLE_LEFT;
		}
	}
	else if (level == MARIO_LEVEL_SMALL)
	{
		if (vx == 0)
		{
			if (nx>0) ani = MARIO_ANI_SMALL_IDLE_RIGHT;
			else ani = MARIO_ANI_SMALL_IDLE_LEFT;
		}
		else if (vx > 0)
		{
			if (IsTouchingGround == true)
				ani = MARIO_ANI_SMALL_WALKING_RIGHT;
			else
				ani = MARIO_ANI_SMALL_JUMPING_RIGHT;
		}

		else
		{
			if (IsTouchingGround == true)
				ani = MARIO_ANI_SMALL_WALKING_LEFT;
			else
				ani = MARIO_ANI_SMALL_JUMPING_LEFT;
		}
	}

	int alpha = 255;
	if (untouchable) alpha = 128;

	animation_set->at(ani)->Render(x, y, alpha);

	//RenderBoundingBox();
}

void CMario::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case MARIO_STATE_WALKING_RIGHT:
		ax = MARIO_WALKING_ACCELERATION;
		if (IsWalkingRight == false)
		{
			vx += MARIO_WALKING_SPEED_START;
			IsWalkingRight = true;
			IsWalkingLeft = false;
		}
		nx = 1;
		break;
	case MARIO_STATE_WALKING_LEFT: 
		ax = -MARIO_WALKING_ACCELERATION;
		if (IsWalkingLeft == false)
		{
			vx += -MARIO_WALKING_SPEED_START;
			IsWalkingLeft = true;
			IsWalkingRight = false;
		}

		nx = -1;
		break;
	case MARIO_STATE_JUMP:
		// TODO: need to check if Mario is *current* on a platform before allowing to jump again
			vy = -MARIO_JUMP_SPEED_Y;
			ny = -1;
		break; 
	case MARIO_STATE_IDLE:
		if (IsTouchingGround == true )
			if (vx > 0)
				ax = -MARIO_WALKING_FRICTION;
			else if (vx < 0)
				ax = MARIO_WALKING_FRICTION;
			else ax = 0;
		else
			if (vx > 0 && nx >0)
				ax = -MARIO_WALKING_ACCELERATION;
			else if (vx < 0 && nx <0)
				ax = MARIO_WALKING_ACCELERATION;
			else ax = 0;
		IsWalkingLeft = IsWalkingRight = false;
		break;
	case MARIO_STATE_DIE:
		vy = -MARIO_DIE_DEFLECT_SPEED;
		break;
	}
}

void CMario::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x-1;
	top = y; 

	if (level==MARIO_LEVEL_BIG)
	{
		right = left + MARIO_BIG_BBOX_WIDTH;
		bottom = top + MARIO_BIG_BBOX_HEIGHT;
	}
	else
	{
		right = left + MARIO_SMALL_BBOX_WIDTH ;
		bottom = top + MARIO_SMALL_BBOX_HEIGHT;
	}
}

/*
	Reset Mario status to the beginning state of a scene
*/
void CMario::Reset()
{
	SetState(MARIO_STATE_IDLE);
	SetLevel(MARIO_LEVEL_BIG);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
}

