#include "Plant_Normal.h"
#include "Mario.h"
#include "PlayScence.h"
#include "Utils.h"


CPlant_Normal::CPlant_Normal(float _x, float _y, float _limit_y, int _type) : CPlant(_x, _y, _limit_y, _type)
{
	SetState(PLANT_NORMAL_STATE_SLEEPING);
}

CPlant_Normal::~CPlant_Normal()
{
	
}

void CPlant_Normal::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + PLANT_NORMAL_BBOX_WIDTH;
	bottom = y + PLANT_NORMAL_BBOX_HEIGHT;
}

void CPlant_Normal::Render()
{
	if (IsInCamera() == false || state == PLANT_NORMAL_STATE_SLEEPING)
		return;
	int ani = -1;
	if (type == PLANT_NORMAL_TYPE_RED)
		ani = PLANT_NORMAL_ANI_RED;
	else if (type == PLANT_NORMAL_TYPE_GREEN)
		ani = PLANT_NORMAL_ANI_GREEN;

	animation_set->at(ani)->Render(x, y);
}



void CPlant_Normal::UpdateInLoop()
{
	if (state == PLANT_NORMAL_STATE_SLEEPING && GetTickCount64() - sleep_start > PLANT_NORMAL_SLEEPING_TIME)
		SetState(PLANT_NORMAL_STATE_MOVING_UP);
	else if (state == PLANT_NORMAL_STATE_MOVING_UP && y <= limit_y)
	{
		y = limit_y;
		SetState(PLANT_NORMAL_STATE_ATTACKING);
	}
	else if (state == PLANT_NORMAL_STATE_ATTACKING)
	{
		if (GetTickCount64() - attacking_start > PLANT_NORMAL_ATTACKING_TIME)
			SetState(PLANT_NORMAL_STATE_MOVING_DOWN);
	}
	else if (state == PLANT_NORMAL_STATE_MOVING_DOWN && y >= start_y)
	{
		y = start_y;
		SetState(PLANT_NORMAL_STATE_SLEEPING);
	}
}

bool CPlant_Normal::IsUpdatable()
{
	if (!IsInCamera())
		return false;
	else
	{
		if (state == PLANT_NORMAL_STATE_SLEEPING)
		{
			CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
			float ml, mt, mr, mb;
			float pl, pt, pr, pb;
			mario->GetBoundingBox(ml, mt, mr, mb);
			GetBoundingBox(pl, pt, pr, pb);
			float mmid = (ml + mr) / 2;
			float pmid = (pl + pr) / 2;
			if (mb>limit_y)
			{
				if (mmid > pmid)
				{
					if (ml - pr < PLANT_NORMAL_MIN_DISTANCE_FROM_MARIO)
						return false;
				}
				else
				{
					if (pl - mr < PLANT_NORMAL_MIN_DISTANCE_FROM_MARIO)
						return false;
				}
			}
		}
	}
	return true;
}

void CPlant_Normal::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (!IsUpdatable())
	{
		if (state != PLANT_NORMAL_STATE_SLEEPING)
		{
			SetState(PLANT_NORMAL_STATE_SLEEPING);
			y = start_y;
		}
		return;
	}
	CGameObject::Update(dt, coObjects);
	UpdateInLoop();

	x += dx;
	y += dy;

}

void CPlant_Normal::SetState(int _state)
{
	CGameObject::SetState(_state);
	switch (_state)
	{
	case PLANT_NORMAL_STATE_MOVING_UP:
		vy = -PLANT_NORMAL_MOVING_SPEED_Y;
		break;
	case PLANT_NORMAL_STATE_MOVING_DOWN:
		vy = PLANT_NORMAL_MOVING_SPEED_Y;
		break;
	case PLANT_NORMAL_STATE_ATTACKING:
		vy = 0;
		this->attacking_start = GetTickCount64();
		break;
	case PLANT_NORMAL_STATE_SLEEPING:
		vy = 0;
		this->sleep_start = GetTickCount64();
		break;
	}
}