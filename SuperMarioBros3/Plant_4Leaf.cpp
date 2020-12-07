#include "Plant_4Leaf.h"
#include "Mario.h"
#include "PlayScence.h"
#include <math.h>
#include "Utils.h"

const double PI = 3.141592653589793;

CPlant_4Leaf::CPlant_4Leaf(float _x, float _y, float _limit_y, int _type): CPlant(_x,_y,_limit_y,_type)
{
	angle = PLANT_4LEAF_ANGLE_45;
	SetState(PLANT_4LEAF_STATE_SLEEPING);
}
CPlant_4Leaf::~CPlant_4Leaf()
{
	delete bullet;
}

void CPlant_4Leaf::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + PLANT_4LEAF_BBOX_WIDTH;
	bottom = y + PLANT_4LEAF_BBOX_HEIGHT;
}

void CPlant_4Leaf::Render()
{
	if (IsInCamera() == false || state == PLANT_4LEAF_STATE_SLEEPING)
		return;
	int ani = -1;
	if (type == PLANT_4LEAF_TYPE_RED)
	{
		if (state == PLANT_4LEAF_STATE_MOVING_DOWN|| state == PLANT_4LEAF_STATE_MOVING_UP)
		{
			switch (angle)
			{
			case PLANT_4LEAF_ANGLE_135:
			case PLANT_4LEAF_ANGLE_225:
			case PLANT_4LEAF_ANGLE_150:
			case PLANT_4LEAF_ANGLE_210:
				ani = PLANT_4LEAF_ANI_RED_MOVING_225_DEGREES;
				break;
			case PLANT_4LEAF_ANGLE_30:
			case PLANT_4LEAF_ANGLE_45:
			case PLANT_4LEAF_ANGLE_330:
			case PLANT_4LEAF_ANGLE_315:
				ani = PLANT_4LEAF_ANI_RED_MOVING_315_DEGREES;
				break;
			}
		}
		else
		{
			switch (angle)
			{
			case PLANT_4LEAF_ANGLE_45:
			case PLANT_4LEAF_ANGLE_30:
				ani = PLANT_4LEAF_ANI_RED_KEEPING_STILL_45_DEGREES;
				break;
			case PLANT_4LEAF_ANGLE_135:
			case PLANT_4LEAF_ANGLE_150:
				ani = PLANT_4LEAF_ANI_RED_KEEPING_STILL_135_DEGREES;
				break;
			case PLANT_4LEAF_ANGLE_225:
			case PLANT_4LEAF_ANGLE_210:
				ani = PLANT_4LEAF_ANI_RED_KEEPING_STILL_225_DEGREES;
				break;
			case PLANT_4LEAF_ANGLE_315:
			case PLANT_4LEAF_ANGLE_330:
				ani = PLANT_4LEAF_ANI_RED_KEEPING_STILL_315_DEGREES;
				break;
			}
		}
	}
	else if(type == PLANT_4LEAF_TYPE_GREEN)
	{

	}
	DebugOut(L"[INFO] angle: %d \n", angle);
	DebugOut(L"[INFO] state: %d \n", state);
	animation_set->at(ani)->Render(x, y);
}

int CPlant_4Leaf::CalculatePositionInComparisonToMario()
{
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	float ml, mt, mr, mb;
	float pl, pt, pr, pb;
	mario->GetBoundingBox(ml, mt, mr, mb);
	GetBoundingBox(pl, pt, pr, pb);
	float mm = (ml + mr) / 2;
	float pm = (pl + pr) / 2;
	if (pm > mm) // mario is on plant's left
	{
		if (mr <= pl || mb < pt || mt < pb)
			return PLANT_4LEAF_IS_ON_MARIOS_RIGHT;
		else
			return PLANT_4LEAF_IS_INTERSECT_WITH_MARIO_ON_THE_RIGHT;
	}
	else //mario is on plant's right
	{
		if (ml >= pr || mb < pt || mt < pb)
			return PLANT_4LEAF_IS_ON_MARIOS_LEFT;
		else
			return PLANT_4LEAF_IS_INTERSECT_WITH_MARIO_ON_THE_LEFT;
	}
}
void CPlant_4Leaf::CalculateAngle()
{
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	float ml, mt, mr, mb;
	float pl, pt, pr, pb;
	mario->GetBoundingBox(ml, mt, mr, mb);
	GetBoundingBox(pl, pt, pr, pb);
	float denta_y = (mt + mb) / 2 - ((pt + pb) / 2 + pt) / 2;
	float denta_x = (ml + mr) / 2 -(pl + pr) / 2;
	double tan = denta_y / denta_x;
	double ang = atan(tan) * 180 / PI;
	int pos = CalculatePositionInComparisonToMario();
	if (pos == PLANT_4LEAF_IS_ON_MARIOS_LEFT || pos == PLANT_4LEAF_IS_INTERSECT_WITH_MARIO_ON_THE_LEFT)
	{
		if (ang >= 0 && ang < 30)
			angle = PLANT_4LEAF_ANGLE_30;
		else if (ang >= 30)
			angle = PLANT_4LEAF_ANGLE_45;
		else if (ang < -30)
			angle = PLANT_4LEAF_ANGLE_315;
		else
			angle = PLANT_4LEAF_ANGLE_330;
	}
	else
	{
		if (ang >= 0 && ang < 30)
			angle = PLANT_4LEAF_ANGLE_210;
		else if (ang >= 30)
			angle = PLANT_4LEAF_ANGLE_225;
		else if (ang < -30)
			angle = PLANT_4LEAF_ANGLE_135;
		else
			angle = PLANT_4LEAF_ANGLE_150;
	}

}

void CPlant_4Leaf::UpdateFlagBaseOnTime()
{
	if (GetTickCount64() - shoot_start > PLANT_4LEAF_SHOOTING_BULLET_TIME)
		IsShooting = false;
	if (GetTickCount64() - sleep_start > PLANT_4LEAF_SLEEPING_TIME)
		IsSleeping = false;
}
void CPlant_4Leaf::UpdateInLoop()
{
	if (state == PLANT_4LEAF_STATE_SLEEPING && GetTickCount64() - sleep_start > PLANT_4LEAF_SLEEPING_TIME)
		SetState(PLANT_4LEAF_STATE_MOVING_UP);
	else if (state == PLANT_4LEAF_STATE_MOVING_UP && y <= limit_y)
	{
		y = limit_y;
		SetState(PLANT_4LEAF_STATE_SHOOTING);
	}
	else if (state == PLANT_4LEAF_STATE_SHOOTING)
	{
		if (GetTickCount64() - shoot_start > PLANT_4LEAF_SHOOTING_BULLET_TIME)
			SetState(PLANT_4LEAF_STATE_MOVING_DOWN);
	}
	else if (state == PLANT_4LEAF_STATE_MOVING_DOWN && y >= start_y)
	{
		y = start_y;
		SetState(PLANT_4LEAF_STATE_SLEEPING);
	}
}
void CPlant_4Leaf::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (IsInCamera() == false)
	{
		if (state != PLANT_4LEAF_STATE_SLEEPING)
		{
			SetState(PLANT_4LEAF_STATE_SLEEPING);
			y = start_y;
		}
		return;
	}
	CGameObject::Update(dt, coObjects);
	UpdateInLoop();
	if(state == PLANT_4LEAF_STATE_SHOOTING)
		CalculateAngle();
	x += dx;
	y += dy;
}

void CPlant_4Leaf::SetState(int _state)
{
	CGameObject::SetState(_state);
	switch (_state)
	{
	case PLANT_4LEAF_STATE_MOVING_UP:
		vy = -PLANT_4LEAF_MOVING_SPEED_Y;
		break;
	case PLANT_4LEAF_STATE_MOVING_DOWN:
		vy = PLANT_4LEAF_MOVING_SPEED_Y;
		break;
	case PLANT_4LEAF_STATE_SHOOTING:
		vy = 0;
		this->shoot_start = GetTickCount64();
		break;
	case PLANT_4LEAF_STATE_SLEEPING:
		vy = 0;
		this->sleep_start = GetTickCount64();
		break;
	}
}