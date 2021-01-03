#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "Mario.h"
#include "Game.h"

#include "Goomba.h"
#include "Portal.h"
#include "Brick.h"
#include "Reward_LevelUp.h"
#include "Koopas.h"
#include "Bullet_Mario.h"
#include "PlayScence.h"
#include "Plant_Fire.h"
#include "Koopa_Small.h"
#include "RewardBox.h"
#include "Plant_Normal.h"
using namespace  std;

CMario::CMario(float x, float y) : CGameObject()
{
	level = MARIO_LEVEL_SMALL;
	untouchable = 0;
	SetState(MARIO_STATE_IDLE);
	changeImminent_start = 0;

	start_x = x; 
	start_y = y; 
	this->x = x; 
	this->y = y;

	jumpStack = 0;
	imminentStack = 0;
	StageOfSwingTail = -1;

	IsReadyJump = true;
	IsTouchingGround = true;
	IsJumping = false;
	IsFalling = false;
	IsFlying = false;

	IsWalkingRight = false;
	IsWalkingLeft = false;

	IsReadyHolding = false;
	IsHolding = false;

	throwFire_start = 0;
}

void CMario::Calculate_vx(DWORD _dt)
{
	vx += ax * _dt;
	//state == walking
	if (abs(vx) > MARIO_WALKING_SPEED_MAX + imminentStack * MARIO_IMMINANT_WALKING_SPEED)
	{
		vx = float(nx * (MARIO_WALKING_SPEED_MAX + imminentStack * MARIO_IMMINANT_WALKING_SPEED));
	}
	//state == idle
	//if (state == MARIO_STATE_IDLE)
	{
		if (nx > 0)
		{
			if (vx < 0)
			{
				vx = 0;
				//DebugOut(L"[INFO] Animation added: %d \n", spriteId);
			}
		}
		else
			if (vx > 0)
				vx = 0;
	}

}
void CMario::Calculate_vy(DWORD _dt)
{
	/*if (level == MARIO_LEVEL_RACCOON && IsRaccoonCanFlyHigh)
	{
		vy = -1.5*MARIO_GRAVITY * _dt;
	}
	else */
	if (IsFallingSlowly)
	{
		vy += MARIO_GRAVITY * _dt;
		if (vy > MARIO_MAX_FALL_SLOWLY_SPEED)
			vy = MARIO_MAX_FALL_SLOWLY_SPEED;
	}
	else
	{
		if (level == MARIO_LEVEL_RACCOON && IsRaccoonCanFlyHigh)
			vy += MARIO_RACCOON_FLY_GRAVITY_COEFFICIENT * MARIO_GRAVITY * _dt;
		else
			vy += MARIO_GRAVITY * _dt;


		if (vy > MARIO_MAX_FALL_SPEED)
			vy = MARIO_MAX_FALL_SPEED;
	}
}
void CMario::UpdateFlagBaseOnTime()
{
	// reset untouchable timer if untouchable time has passed
	if (state != MARIO_STATE_DIE)
	{
		if (GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
		{
			untouchable_start = 0;
			untouchable = 0;
		}
	}
	if (GetTickCount64() - throwFire_start > MARIO_PERFORM_THROW_TIME)
		IsThrowing = false;

	if (GetTickCount64() - swingTail_start > MARIO_PERFORM_SWING_TAIL_TIME)
		IsSwingTail = false;

	if (GetTickCount64() - kick_start > MARIO_KICKING_TIME)
		IsKicking = false;

	if (GetTickCount64() - fallSlowly_start > MARIO_FALLING_SLOWLY_TIME)
		IsFallingSlowly = false;
	if (GetTickCount64() - canFlyHigh_start > MARIO_RACCOON_CAN_FLY_TIME)
		IsRaccoonCanFlyHigh = false;
}
void CMario::UpdateBullets(DWORD _dt, vector<LPGAMEOBJECT>* coObjects)
{
	for (int i = 0; unsigned(i) < Bullets.size(); i++)
	{
		CBullet_Mario* bullet = (CBullet_Mario*)Bullets[i];
		if (bullet->GetState() != BULLET_STATE_EXPLODING)
			bullet->Update(_dt, coObjects);
		else if (GetTickCount64() - bullet->GetStartExplode_time() > BULLET_MARIO_EXPLOSION_TIME)
		{
			//CGameObject* bul = Bullets[i];
			Bullets.erase(Bullets.begin() + i);
			delete bullet;
		}
	}
}
void CMario::UpdateWhenSwingTail()
{
	if (IsSwingTail)
	{
		int stage = (GetTickCount64() - swingTail_start)/MARIO_EACH_STAGE_IN_SWING_TAIL_TIME;
		if(nx>0)
			switch (stage)
			{
			case 0:
				if (StageOfSwingTail == 0)
				{
					x = x + MARIO_RACCOON_TAIL_BBOX_WIDTH - 2;
					StageOfSwingTail++;
					//DebugOut(L"[INFO] case 0, x = %d \n", x);
				}
				break;
			case 1:
				if (StageOfSwingTail == 1)
					StageOfSwingTail++;
				break;
			case 2:
				if (StageOfSwingTail == 2)
					StageOfSwingTail++;
				break;
			case 3:
				if (StageOfSwingTail == 3)
				{
					x = x - MARIO_RACCOON_TAIL_BBOX_WIDTH + 2;
					//DebugOut(L"[INFO] case 3, x = %d \n", x);
					StageOfSwingTail = 0;
				}
				break;
			}
		else
			switch (stage)
			{
			case 0:
				if (StageOfSwingTail == 0)
					StageOfSwingTail++;
				break;
			case 1:
				if (StageOfSwingTail == 1)
				{
					x = x - MARIO_RACCOON_TAIL_BBOX_WIDTH + 2;
					StageOfSwingTail++;
				}
				break;
			case 2:
				if (StageOfSwingTail == 2)
				{
					x = x + MARIO_RACCOON_TAIL_BBOX_WIDTH - 2;
					StageOfSwingTail++;
				}
				break;
			case 3:
				if (StageOfSwingTail == 3)
					StageOfSwingTail = 0;
				break;
			}
	}
}


void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt);
	UpdateBullets(dt, coObjects);
	Calculate_vy(dt);
	Calculate_vx(dt);
	UpdateFlagBaseOnTime();

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();

	// turn off collision when die 
	if (state!=MARIO_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	
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

		float mleft, mtop, mright, mbottom;
		GetBoundingBox(mleft, mtop, mright, mbottom);
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<CBrick*>(e->obj))
			{
				CBrick* brick = dynamic_cast<CBrick*>(e->obj);
				switch (brick->GetType())
				{
				case BRICK_TYPE_PLATFORM:
					BasicCollision(min_tx, min_ty, e->nx, e->ny, x0, y0);
					break;
				case BRICK_TYPE_BIG_BLOCK:
				{
					if (e->ny == -1)
					{
						BasicCollision(min_tx, min_ty, e->nx, e->ny, x0, y0);
					}
					else
					{
						x = x0 + dx;
						y = y0 + dy;
					}
					break;
				}
				}
				
			}
			else if(dynamic_cast<CRewardBox*>(e->obj))
			{
				CRewardBox* box = dynamic_cast<CRewardBox*>(e->obj);
				BasicCollision(min_tx, min_ty, e->nx, e->ny, x0, y0);
				if (e->ny == 1)
				{
					box->BeCollidedOnTheBottom();
				}
			}
			else if(dynamic_cast<CGoomba*>(e->obj))
			{
				CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
				// jump on top >> kill Goomba and deflect a bit 
				if (e->ny != 0)
				{
					if (e->ny < 0)
					{
						BasicCollision(min_tx, min_ty, e->nx, e->ny, x0, y0);
						if (goomba->GetState() != GOOMBA_STATE_DIE_Y)
						{
							goomba->BeDamaged_Y();
 							vy = -MARIO_JUMP_DEFLECT_SPEED;
							IsJumping = true;
							ny = -1;
						}
					}
					else
					{
						if (untouchable == 0)
								BeDamaged();
					}
				}
				else if (e->nx != 0)
				{
					if (untouchable == 0)
						if (!IsSwingTail)
							BeDamaged();
						else
							goomba->SetState(GOOMBA_STATE_DIE_X);
				}
			}
			else if (dynamic_cast<CCoin*>(e->obj)) // if e->obj is Coin
			{
				CCoin* coin = dynamic_cast<CCoin*>(e->obj);
				coin->isEnable = false;
			}
			else if (dynamic_cast<CKoopa_Small*>(e->obj))
			{
				CKoopa_Small* koopa = dynamic_cast<CKoopa_Small*>(e->obj);
				if (e->nx != 0)
				{
					if (koopa->state == KOOPA_SMALL_STATE_IDLE) //holdable when turtoiseshell Idle
					{
						if (IsReadyHolding == true)
						{
							IsHolding = true;
							koopa->IsBeingHeld = true;
							koopa->SetState(KOOPA_SMALL_STATE_IDLE);
							IsReadyHolding = false;
						}
						else
						{
							IsKicking = true;
							kick_start = GetTickCount64();
							koopa->BeKicked(this->nx);
						}
					}
					else //die when turtoiseshell run
					{
						if (untouchable == 0)
							BeDamaged();
					}
				}
				if (e->ny != 0)
				{
					if (e->ny < 0)
					{
						koopa->BeDamaged_Y();
						vy = -MARIO_JUMP_DEFLECT_SPEED;
						IsJumping = true;
						ny = -1;
					}
					else if (untouchable == 0)
						this->BeDamaged();
				}
			}
			else if (dynamic_cast<CPlant*>(e->obj))
			{
				if (untouchable == 0)
					BeDamaged();
			}
			else if (dynamic_cast<CPortal*>(e->obj))
			{
				BasicCollision(min_tx, min_ty, e->nx, e->ny, x0, y0);

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

	UpdateWhenSwingTail();
	if (vy > 0)
	{
		if (IsJumping == true)
		{
			IsJumping = false;
			IsFalling = true;
		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) 
		delete coEvents[i];
	
}

void CMario::BasicCollision(float min_tx, float min_ty, float _nx, float _ny, float x0, float y0)
{
	if (_nx !=0 )
	{
		//if (ceil(mbottom) -1  != otop)
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
			IsJumping = false;
			IsFalling = false;
			IsFlying = false;
			//this->SetState(MARIO_STATE_IDLE);
		}
		else if (_ny == 1)
		{
			IsTouchingGround = false;
			IsReadyJump = false;

		}
	}
	else
	{
		IsTouchingGround = false;
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
			{
				if (IsRunning)
					ani = MARIO_ANI_BIG_RUNNING_LEFT;
				else
					ani = MARIO_ANI_BIG_WALKING_LEFT;
			}
		else if (state == MARIO_STATE_WALKING_RIGHT)
			if (vx <= 0)
				ani = MARIO_ANI_BIG_SKIDDING_RIGHT;
			else
			{
				if (IsRunning)
					ani = MARIO_ANI_BIG_RUNNING_RIGHT;
				else
					ani = MARIO_ANI_BIG_WALKING_RIGHT;
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
		if (IsJumping == true || IsFalling == true) //if (state == MARIO_STATE_JUMP)
		{
			if (nx > 0)
			{
				if (IsFlying)
					ani = MARIO_ANI_BIG_RUNNING_JUMP_RIGHT;
				else
					ani = MARIO_ANI_BIG_JUMPING_RIGHT;
			}

			else if (nx < 0)
			{
				if (IsFlying)
					ani = MARIO_ANI_BIG_RUNNING_JUMP_LEFT;
				else
					ani = MARIO_ANI_BIG_JUMPING_LEFT;
			}
		}
		if (IsHolding)
		{
			if (vx > 0)
			{
				if (IsJumping || IsFalling)
					ani = MARIO_ANI_BIG_HOLDING_JUMP_RIGHT;
				else
					ani = MARIO_ANI_BIG_HOLDING_RIGHT;

			}
			else if (vx < 0)
				if (IsJumping || IsFalling)
					ani = MARIO_ANI_BIG_HOLDING_JUMP_LEFT;
				else
					ani = MARIO_ANI_BIG_HOLDING_LEFT;
			else
			{
				if (IsJumping || IsFalling)
				{
					if (nx > 0)
						ani = MARIO_ANI_BIG_HOLDING_JUMP_RIGHT;
					else
						ani = MARIO_ANI_BIG_HOLDING_JUMP_LEFT;
				}
				else
				{
					if (nx > 0)
						ani = MARIO_ANI_BIG_HOLDING_IDLE_RIGHT;
					else
						ani = MARIO_ANI_BIG_HOLDING_IDLE_LEFT;
				}
			}
		}
		else if (IsKicking == true)
		{
			if (nx > 0)
				ani = MARIO_ANI_BIG_KICKING_RIGHT;
			else ani = MARIO_ANI_BIG_KICKING_LEFT;
		}
	}
	else if (level == MARIO_LEVEL_SMALL)
	{
		if (state == MARIO_STATE_WALKING_LEFT)
			if (vx >= 0)
				ani = MARIO_ANI_SMALL_SKIDDING_LEFT;
			else
			{
				if (IsRunning)
					ani = MARIO_ANI_SMALL_RUNNING_LEFT;
				else
					ani = MARIO_ANI_SMALL_WALKING_LEFT;
			}
		else if (state == MARIO_STATE_WALKING_RIGHT)
			if (vx <= 0)
				ani = MARIO_ANI_SMALL_SKIDDING_RIGHT;
			else
			{
				if (IsRunning)
					ani = MARIO_ANI_SMALL_RUNNING_RIGHT;
				else
					ani = MARIO_ANI_SMALL_WALKING_RIGHT;
			}

		else //if (state == MARIO_STATE_IDLE)
		{
			if (vx > 0)
				ani = MARIO_ANI_SMALL_WALKING_RIGHT;
			else if (vx < 0)
				ani = MARIO_ANI_SMALL_WALKING_LEFT;
			else
				if (nx > 0)
					ani = MARIO_ANI_SMALL_IDLE_RIGHT;
				else
					ani = MARIO_ANI_SMALL_IDLE_LEFT;
		}
		if (IsJumping == true || IsFalling == true) //if (state == MARIO_STATE_JUMP)
		{
			if (nx > 0)
			{
				if (IsFlying)
					ani = MARIO_ANI_SMALL_RUNNING_JUMP_RIGHT;
				else
					ani = MARIO_ANI_SMALL_JUMPING_RIGHT;
			}

			else if (nx < 0)
			{
				if (IsFlying)
					ani = MARIO_ANI_SMALL_RUNNING_JUMP_LEFT;
				else
					ani = MARIO_ANI_SMALL_JUMPING_LEFT;
			}
		}
		if (IsHolding)
		{
			if (vx > 0)
			{
				if (IsJumping || IsFalling)
					ani = MARIO_ANI_SMALL_HOLDING_JUMP_RIGHT;
				else
					ani = MARIO_ANI_SMALL_HOLDING_RIGHT;

			}
			else if (vx < 0)
				if (IsJumping || IsFalling)
					ani = MARIO_ANI_SMALL_HOLDING_JUMP_LEFT;
				else
					ani = MARIO_ANI_SMALL_HOLDING_LEFT;
			else
			{
				if (IsJumping || IsFalling)
				{
					if (nx > 0)
						ani = MARIO_ANI_SMALL_HOLDING_JUMP_RIGHT;
					else
						ani = MARIO_ANI_SMALL_HOLDING_JUMP_LEFT;
				}
				else
				{
					if (nx > 0)
						ani = MARIO_ANI_SMALL_HOLDING_IDLE_RIGHT;
					else
						ani = MARIO_ANI_SMALL_HOLDING_IDLE_LEFT;
				}
			}
		}
		else if (IsKicking == true)
		{
			if (nx > 0)
				ani = MARIO_ANI_SMALL_KICKING_RIGHT;
			else ani = MARIO_ANI_SMALL_KICKING_LEFT;
		}
	}
	else if (level == MARIO_LEVEL_RACCOON)
	{
	if (state == MARIO_STATE_WALKING_LEFT)
		if (vx >= 0)
			ani = MARIO_ANI_RACCOON_SKIDDING_LEFT;
		else
		{
			if (IsRunning)
				ani = MARIO_ANI_RACCOON_RUNNING_LEFT;
			else
				ani = MARIO_ANI_RACCOON_WALKING_LEFT;
		}
	else if (state == MARIO_STATE_WALKING_RIGHT)
		if (vx <= 0)
			ani = MARIO_ANI_RACCOON_SKIDDING_RIGHT;
		else
		{
			if (IsRunning)
				ani = MARIO_ANI_RACCOON_RUNNING_RIGHT;
			else
				ani = MARIO_ANI_RACCOON_WALKING_RIGHT;
		}

	else //if (state == MARIO_STATE_IDLE)
	{
		if (vx > 0)
			ani = MARIO_ANI_RACCOON_WALKING_RIGHT;
		else if (vx < 0)
			ani = MARIO_ANI_RACCOON_WALKING_LEFT;
		else
			if (nx > 0)
				ani = MARIO_ANI_RACCOON_IDLE_RIGHT;
			else
				ani = MARIO_ANI_RACCOON_IDLE_LEFT;
	}
	if (IsJumping) //if (state == MARIO_STATE_JUMP)
	{
		if (nx > 0)
		{
			if (IsFlying)
				ani = MARIO_ANI_RACCOON_FLYING_RIGHT;
			else
				ani = MARIO_ANI_RACCOON_JUMPING_RIGHT;
		}

		else if (nx < 0)
		{
			if (IsFlying)
				ani = MARIO_ANI_RACCOON_FLYING_LEFT;
			else
				ani = MARIO_ANI_RACCOON_JUMPING_LEFT;
		}
	}
	else if (IsFalling)
	{
		if (nx > 0)
		{
			if (IsFallingSlowly)
				ani = MARIO_ANI_RACCOON_WAGGING_RIGHT;
			else
			{
				if (IsFlying)
					ani = MARIO_ANI_RACCOON_FLYING_RIGHT;
				else
					ani = MARIO_ANI_RACCOON_FALLING_RIGHT;
			}
		}

		else if (nx < 0)
		{
			if (IsFallingSlowly)
				ani = MARIO_ANI_RACCOON_WAGGING_LEFT;
			else
			{
				if (IsFlying)
					ani = MARIO_ANI_RACCOON_FLYING_LEFT;
				else
					ani = MARIO_ANI_RACCOON_FALLING_LEFT;
			}
		}
	}
	if (IsHolding)
	{
		if (vx > 0)
		{
			if (IsJumping || IsFalling)
				ani = MARIO_ANI_RACCOON_HOLDING_JUMP_RIGHT;
			else
				ani = MARIO_ANI_RACCOON_HOLDING_RIGHT;

		}
		else if (vx < 0)
			if (IsJumping || IsFalling)
				ani = MARIO_ANI_RACCOON_HOLDING_JUMP_LEFT;
			else
				ani = MARIO_ANI_RACCOON_HOLDING_LEFT;
		else
		{
			if (IsJumping || IsFalling)
			{
				if (nx > 0)
					ani = MARIO_ANI_RACCOON_HOLDING_JUMP_RIGHT;
				else
					ani = MARIO_ANI_RACCOON_HOLDING_JUMP_LEFT;
			}
			else
			{
				if (nx > 0)
					ani = MARIO_ANI_RACCOON_HOLDING_IDLE_RIGHT;
				else
					ani = MARIO_ANI_RACCOON_HOLDING_IDLE_LEFT;
			}
		}
	}
	else if (IsKicking == true)
	{
		if (nx > 0)
			ani = MARIO_ANI_RACCOON_KICKING_RIGHT;
		else ani = MARIO_ANI_RACCOON_KICKING_LEFT;
	}
	else if (IsSwingTail)
	{
		if (nx > 0)
		{
			if (IsTouchingGround)
				ani = MARIO_ANI_RACCOON_SWING_TAIL_1_RIGHT;
			else
				ani = MARIO_ANI_RACCOON_SWING_TAIL_2_RIGHT;
		}
		else
		{
			if (IsTouchingGround)
				ani = MARIO_ANI_RACCOON_SWING_TAIL_1_LEFT;
			else
				ani = MARIO_ANI_RACCOON_SWING_TAIL_2_LEFT;
		}
	}
	}
	else if (level == MARIO_LEVEL_FIRE)
	{
	if (state == MARIO_STATE_WALKING_LEFT)
		if (vx >= 0)
			ani = MARIO_ANI_FIRE_SKIDDING_LEFT;
		else
		{
			if (IsRunning)
				ani = MARIO_ANI_FIRE_RUNNING_LEFT;
			else
				ani = MARIO_ANI_FIRE_WALKING_LEFT;
		}
			
	else if (state == MARIO_STATE_WALKING_RIGHT)
		if (vx <= 0)
			ani = MARIO_ANI_FIRE_SKIDDING_RIGHT;
		else
		{
			if (IsRunning)
				ani = MARIO_ANI_FIRE_RUNNING_RIGHT;
			else
				ani = MARIO_ANI_FIRE_WALKING_RIGHT;
		}

	else //if (state == MARIO_STATE_IDLE)
	{
		if (vx > 0)
			ani = MARIO_ANI_FIRE_WALKING_RIGHT;
		else if (vx < 0)
			ani = MARIO_ANI_FIRE_WALKING_LEFT;
		else
			if (nx > 0)
				ani = MARIO_ANI_FIRE_IDLE_RIGHT;
			else
				ani = MARIO_ANI_FIRE_IDLE_LEFT;
	}
	if (IsJumping == true || IsFalling == true) //if (state == MARIO_STATE_JUMP)
	{
		if (nx > 0)
		{
			if (IsFlying)
				ani = MARIO_ANI_FIRE_RUNNING_JUMP_RIGHT;
			else
				ani = MARIO_ANI_FIRE_JUMPING_RIGHT;
		}
			
		else if (nx < 0)
		{
			if (IsFlying)
				ani = MARIO_ANI_FIRE_RUNNING_JUMP_LEFT;
			else
				ani = MARIO_ANI_FIRE_JUMPING_LEFT;
		}
	}
	if (IsHolding)
	{
		if (vx > 0)
		{
			if (IsJumping || IsFalling)
				ani = MARIO_ANI_FIRE_HOLDING_JUMP_RIGHT;
			else
				ani = MARIO_ANI_FIRE_HOLDING_RIGHT;

		}
		else if (vx < 0)
			if (IsJumping || IsFalling)
				ani = MARIO_ANI_FIRE_HOLDING_JUMP_LEFT;
			else
				ani = MARIO_ANI_FIRE_HOLDING_LEFT;
		else
		{
			if (IsJumping || IsFalling)
			{
				if (nx > 0)
					ani = MARIO_ANI_FIRE_HOLDING_JUMP_RIGHT;
				else
					ani = MARIO_ANI_FIRE_HOLDING_JUMP_LEFT;
			}
			else
			{
				if (nx > 0)
					ani = MARIO_ANI_FIRE_HOLDING_IDLE_RIGHT;
				else
					ani = MARIO_ANI_FIRE_HOLDING_IDLE_LEFT;
			}
		}

	}
	if (IsThrowing)
	{
		if (nx > 0)
			ani = MARIO_ANI_FIRE_THROWING_RIGHT;
		else ani = MARIO_ANI_FIRE_THROWING_LEFT;
	}
	else if(IsKicking == true)
	{
		if (nx > 0)
			ani = MARIO_ANI_FIRE_KICKING_RIGHT;
		else ani = MARIO_ANI_FIRE_KICKING_LEFT;
	}
	}

	int alpha = 255;
	if (untouchable && state!= MARIO_STATE_DIE) alpha = 128;

	animation_set->at(ani)->Render(x, y, alpha);
	//Render Bullet
	for (int i = 0; (unsigned)i < Bullets.size(); i++)
	{
		Bullets[i]->Render();
	}

	//RenderBoundingBox();
}

void CMario::SetState(int state)
{
	if (this->state == MARIO_STATE_DIE)
		return;
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
		IsJumping = true;
		vy = -MARIO_JUMP_SPEED_Y;
		ny = -1;
		if (imminentStack == MARIO_MAX_IMMINENT_STACKS)
			IsFlying = true;
		break; 
	case MARIO_STATE_IDLE:
		if (IsTouchingGround == true)
		{
			if (vx > 0)
				ax = -MARIO_WALKING_FRICTION;
			else if (vx < 0)
				ax = MARIO_WALKING_FRICTION;
			else ax = 0;
		}
		else
		{
			if (vx > 0 && nx > 0)
				ax = -MARIO_WALKING_ACCELERATION;
			else if (vx < 0 && nx < 0)
				ax = MARIO_WALKING_ACCELERATION;
			else ax = 0;
		}
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
	else if(level == MARIO_LEVEL_SMALL)
	{
		right = left + MARIO_SMALL_BBOX_WIDTH ;
		bottom = top + MARIO_SMALL_BBOX_HEIGHT;
	}
	else if(level == MARIO_LEVEL_RACCOON)
	{
		if (nx > 0)
		{
			if (IsSwingTail)
			{
				left = x;
				right = left + MARIO_RACCOON_BBOX_WIDTH + MARIO_RACCOON_TAIL_BBOX_WIDTH;
			}
			else
			{
				left = x + MARIO_RACCOON_TAIL_BBOX_WIDTH;
				right = left + MARIO_RACCOON_BBOX_WIDTH;
			}
		}
		else
		{
			if (IsSwingTail)
			{
				left = x  - MARIO_RACCOON_TAIL_BBOX_WIDTH;
				right = x + MARIO_RACCOON_BBOX_WIDTH + MARIO_RACCOON_TAIL_BBOX_WIDTH;
			}
			else
			{
				left = x ;
				right = left + MARIO_RACCOON_BBOX_WIDTH;
			}
		}
		bottom = top + MARIO_RACCOON_BBOX_HEIGHT;
	}
	else 
	{
		right = left + MARIO_FIRE_BBOX_WIDTH;
		bottom = top + MARIO_FIRE_BBOX_HEIGHT;
	}
}

/*
	Reset Mario status to the beginning state of a scene
*/
void CMario::Reset()
{
	this->state = MARIO_STATE_IDLE;
	SetState(MARIO_STATE_IDLE);
	SetLevel(MARIO_LEVEL_BIG);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
}
void CMario::StartThrowFire()
{
	if (GetTickCount64() - throwFire_start >= MARIO_THROWING_TIME)
	{

		IsThrowing = true;
		float l, t, r, b;
		GetBoundingBox(l, t, r, b);
		CBullet_Mario* bullet;
		
		bullet = new CBullet_Mario((r+l)/2, t);
		bullet->nx = this->nx;
		bullet->vx =  this->nx * BULLET_MARIO_SPEED_X;
		bullet->vy = BULLET_MARIO_FIRST_SPEED_Y;
		Bullets.push_back(bullet);
		throwFire_start = GetTickCount64();
	}
}

//void CMario::changeImminent()
//{
//	if (IsTouchingGround)
//	{
//		if (abs(vx) >= MARIO_WALKING_SPEED_MAX - 0.001)
//		{
//			if (GetTickCount64() - changeImminent_start >= MARIO_CHANGE_IMMINENT_TIME)
//			{
//				imminentStack++;
//				if (imminentStack > MARIO_MAX_IMMINENT_STACKS - 1)
//				{
//					imminentStack = MARIO_MAX_IMMINENT_STACKS;
//					IsRunning = true;
//				}
//				changeImminent_start = GetTickCount64();
//			}
//		}
//		else
//		{
//			downImminent();
//		}
//	}
//}
void CMario::upImminent()
{
	if (IsTouchingGround)
	{
		if (abs(vx) >= MARIO_WALKING_SPEED_MAX - 0.001)
		{
			if (GetTickCount64() - changeImminent_start >= MARIO_CHANGE_IMMINENT_TIME)
			{
				imminentStack++;
				if (imminentStack > MARIO_MAX_IMMINENT_STACKS - 1)
				{
					imminentStack = MARIO_MAX_IMMINENT_STACKS;
					IsRunning = true;
				}
				changeImminent_start = GetTickCount64();
			}
		}
	}
	else if (GetTickCount64() - canFlyHigh_start <= MARIO_RACCOON_CAN_FLY_TIME)
	{
		imminentStack = MARIO_MAX_IMMINENT_STACKS;
	}
}
void CMario::downImminent()
{
	if (GetTickCount64() - canFlyHigh_start <= MARIO_RACCOON_CAN_FLY_TIME)
	{
		imminentStack = MARIO_MAX_IMMINENT_STACKS -1;
	}
	else if (GetTickCount64() - changeImminent_start >= MARIO_CHANGE_IMMINENT_TIME)
	{
		imminentStack--;
		if (imminentStack < 0)
			imminentStack = 0;
		changeImminent_start = GetTickCount64();
	}
	IsRunning = false;
}
void CMario::StartSwingTail()
{
	if (GetTickCount64() - swingTail_start >= MARIO_SWING_TAIL_TIME)
	{

		IsSwingTail = true;
		swingTail_start = GetTickCount64();
		StageOfSwingTail = 0;
	}
}
void CMario::BeDamaged()
{
	switch (level)
	{
	case MARIO_LEVEL_SMALL:
		SetState(MARIO_STATE_DIE);
		untouchable = 1;
		break;
	case MARIO_LEVEL_BIG:
		level = MARIO_LEVEL_SMALL;
		StartUntouchable();
		break;
	case MARIO_LEVEL_RACCOON:
		level = MARIO_LEVEL_BIG;
		StartUntouchable();
		break;
	case MARIO_LEVEL_FIRE:
		break;
	}
}
void CMario::SlowFall()
{
		IsFallingSlowly = true;
		fallSlowly_start = GetTickCount64();
}
void CMario::RaccoonStartFlyHigh()
{
	IsRaccoonCanFlyHigh = true;
	canFlyHigh_start = GetTickCount64();
}
bool CMario::IsRaccoonReadyFly()
{
	if (!IsFlying && imminentStack == MARIO_MAX_IMMINENT_STACKS)
		return true;
	return false;
}

void CMario::UpLevel()
{
	switch (level)
	{
	case MARIO_LEVEL_SMALL:
		SetLevel(MARIO_LEVEL_BIG);
		y = y + MARIO_SMALL_BBOX_HEIGHT - MARIO_BIG_BBOX_HEIGHT;
		break;
	case MARIO_LEVEL_BIG:
	case MARIO_LEVEL_FIRE:
		SetLevel(MARIO_LEVEL_RACCOON);
		break;
	case MARIO_LEVEL_RACCOON:
		break;
	}
}
