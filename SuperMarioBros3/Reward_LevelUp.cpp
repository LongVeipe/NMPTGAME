#include "Reward_LevelUp.h"
#include "Game.h"
#include "Mario.h"
#include "PlayScence.h"
#include <algorithm>
#include "RewardBox.h"
#include "Utils.h"


CReward_LevelUp::CReward_LevelUp(float _x, float _y)
{
	this->x = _x;
	this->y = _y;
	this->start_y = _y;
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	switch (mario->GetLevel())
	{
	case MARIO_LEVEL_SMALL:
		type = REWARD_LEVEL_UP_TYPE_SUPER_MUSHROOM;
		break;
	case MARIO_LEVEL_BIG:
	case MARIO_LEVEL_FIRE:
	case MARIO_LEVEL_RACCOON:
		type = REWARD_LEVEL_UP_TYPE_SUPER_LEAF;
		break;
	}
	isEnable = true;
	SetAnimationSet(CAnimationSets::GetInstance()->Get(REWARD_LEVEL_UP_ANI_SET_ID));
	/*if (type == REWARD_LEVEL_UP_TYPE_SUPER_LEAF)
	{
		LPANIMATION_SET ani_set = animation_sets->Get(REWARD_LEVEL_UP_ANI_SET_ID_SUPER_LEAF);
		this->SetAnimationSet(ani_set);
	}
	else if (type == REWARD_LEVEL_UP_TYPE_SUPER_MUSHROOM)
	{
		LPANIMATION_SET ani_set = animation_sets->Get(REWARD_LEVEL_UP_ANI_SET_ID_SUPER_MUSHROOM);
		this->SetAnimationSet(ani_set);
	}*/
}


void CReward_LevelUp::Render()
{
	if (!isEnable)
		return;
	int ani = -1;

	if (type == REWARD_LEVEL_UP_TYPE_SUPER_LEAF)
	{
		if (state == REWARD_LEVEL_UP_STATE_SHAKING_LEFT)
			ani = REWARD_LEVEL_UP_ANI_SUPER_LEAF_LEFT;
		else
			ani = REWARD_LEVEL_UP_ANI_SUPER_LEAF_RIGHT;
	}
	else if (type == REWARD_LEVEL_UP_TYPE_SUPER_MUSHROOM)
		ani = REWARD_LEVEL_UP_ANI_SUPER_MUSHROOM;

	animation_set->at(ani)->Render(x, y);
}

bool CReward_LevelUp::IsOnTheLeftOfMario()
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
void CReward_LevelUp::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (!isEnable)
		return;
		
	CGameObject::Update(dt, coObjects);

	CalcPotentialCollisionWithMario();
	if (type == REWARD_LEVEL_UP_TYPE_SUPER_LEAF)
	{
		Update_Leaf();
	}
	else
		Update_MushRoom(dt, coObjects);
}

void CReward_LevelUp::Update_Leaf()
{
	x += dx;
	y += dy;
	if (state == REWARD_LEVEL_UP_STATE_JUMPING)
	{
		vy = vy + REWARD_LEVEL_UP_GRAVITY * dt + REWARD_LEVEL_UP_JET * dt;
		if (vy > 0)
		{
			vy = REWARD_LEVEL_UP_GRAVITY * dt;
			SetState(REWARD_LEVEL_UP_STATE_SHAKING_RIGHT);
		}
	}
	else 
	{
		vy = vy - REWARD_LEVEL_UP_JET * dt;
		if (vy < -(10 * REWARD_LEVEL_UP_JET * dt))
		{
			vy = REWARD_LEVEL_UP_GRAVITY * dt;
			if (state == REWARD_LEVEL_UP_STATE_SHAKING_RIGHT)
				SetState(REWARD_LEVEL_UP_STATE_SHAKING_LEFT);
			else
				SetState(REWARD_LEVEL_UP_STATE_SHAKING_RIGHT);

		}
	}
}


void CReward_LevelUp::Update_MushRoom(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	
	if (state == REWARD_LEVEL_UP_STATE_JUMPING)
	{
		if (y + REWARD_LEVEL_UP_SUPER_MUSHROOM_BBOX_HEIGHT >= start_y)
		{
			y += dy;
		}
		else
		{
			if (IsOnTheLeftOfMario())
				SetState(REWARD_LEVEL_UP_STATE_WALKING_LEFT);
			else
				SetState(REWARD_LEVEL_UP_STATE_WALKING_RIGHT);
		}
		return;
	}
	vy += REWARD_LEVEL_UP_GRAVITY * dt;
	if (vy > REWARD_LEVEL_UP_MAX_FALLING_SPEED)
		vy = REWARD_LEVEL_UP_MAX_FALLING_SPEED;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	this->CalcPotentialCollisions(coObjects, coEvents);

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

		float x0 = x;
		float y0 = y;
		x += dx;
		y += dy;
		if (nx != 0)
		{
			this->x = x0+ min_tx * this->dx + nx * 0.1f;
			this->vx = -vx;
		}
		if (ny != 0)
		{
			this->vy = 0;
			this->y = y0 + min_ty * this->dy + ny * 0.1f;

		}

	}

	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CReward_LevelUp::CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents)
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{

		if (!IsInCamera())
			continue;
		LPGAMEOBJECT object = coObjects->at(i);
		if (!dynamic_cast<CBrick*>(object) && !dynamic_cast<CRewardBox*>(object))
			continue;


		LPCOLLISIONEVENT e = SweptAABBEx(object);

		if (e->t > 0 && e->t <= 1.0f)
		{
			float mleft, mtop, mright, mbottom;
			GetBoundingBox(mleft, mtop, mright, mbottom);
			float oleft, otop, obottom, oright;
			e->obj->GetBoundingBox(oleft, otop, oright, obottom);
			if (e->nx != 0)
			{
				if (ceil(mbottom) == otop)
				{
					continue;
				}
			}
			coEvents.push_back(e);
		}
		else
			delete e;
	}

	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);
}

void CReward_LevelUp::CalcPotentialCollisionWithMario()
{
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	LPCOLLISIONEVENT e = SweptAABBEx(mario);

	if (e->t > 0 && e->t <= 1.0f)
	{
		mario->UpLevel();
		isEnable = false;
	}
}
void CReward_LevelUp::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (!isEnable)
		l = t = r = b = 0;
	else if(type == REWARD_LEVEL_UP_TYPE_SUPER_LEAF)
	{
		l = x;
		t = y;
		r = x + REWARD_LEVEL_UP_SUPER_LEAF_BBOX_WIDTH;
		b = y + REWARD_LEVEL_UP_SUPER_LEAF_BBOX_HEIGHT;
	}
	else if (type == REWARD_LEVEL_UP_TYPE_SUPER_MUSHROOM)
	{
		l = x;
		t = y;
		r = x + REWARD_LEVEL_UP_SUPER_MUSHROOM_BBOX_WIDTH;
		b = y + REWARD_LEVEL_UP_SUPER_MUSHROOM_BBOX_HEIGHT;
	}
}

void CReward_LevelUp::SetState(int _state)
{
	CGameObject::SetState(_state);
	switch (state)
	{
	case REWARD_LEVEL_UP_STATE_WALKING_RIGHT:
		vx = REWARD_LEVEL_UP_WALKING_SPEED;
		break;
	case REWARD_LEVEL_UP_STATE_WALKING_LEFT:
		vx = -REWARD_LEVEL_UP_WALKING_SPEED;
		break;
	case REWARD_LEVEL_UP_STATE_JUMPING:
		if(type == REWARD_LEVEL_UP_TYPE_SUPER_LEAF)
			vy = -REWARD_LEVEL_UP_LEAF_JUMP_SPEED_Y;
		else if(type == REWARD_LEVEL_UP_TYPE_SUPER_MUSHROOM)
			vy = -REWARD_LEVEL_UP_MUSHROOM_JUMP_SPEED_Y;
		break;
	case REWARD_LEVEL_UP_STATE_SHAKING_LEFT:
		vx = -REWARD_LEVEL_UP_SHAKING_SPEED_X;
		break;
	case REWARD_LEVEL_UP_STATE_SHAKING_RIGHT:
		vx = REWARD_LEVEL_UP_SHAKING_SPEED_X;
		break;
	}
}

