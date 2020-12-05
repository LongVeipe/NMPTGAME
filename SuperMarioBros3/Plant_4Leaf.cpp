#include "Plant_4Leaf.h"

CPlant_4Leaf::CPlant_4Leaf(float _x, float _y, float _limit_y, int _type): CPlant(_x,_y,_limit_y,_type)
{
	angle = PLANT_4LEAF_ANGLE_45;
	IsMoving = false;
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
	int ani = -1;
	if (type == PLANT_4LEAF_TYPE_RED)
	{
		if (IsMoving)
		{
			switch (angle)
			{
			case PLANT_4LEAF_ANGLE_45:
				ani = PLANT_4LEAF_ANI_RED_MOVING_45_DEGREES;
				break;
			case PLANT_4LEAF_ANGLE_135:
				ani = PLANT_4LEAF_ANI_RED_MOVING_135_DEGREES;
				break;
			case PLANT_4LEAF_ANGLE_225:
				ani = PLANT_4LEAF_ANI_RED_MOVING_225_DEGREES;
				break;
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
				ani = PLANT_4LEAF_ANI_RED_KEEPING_STILL_45_DEGREES;
				break;
			case PLANT_4LEAF_ANGLE_135:
				ani = PLANT_4LEAF_ANI_RED_KEEPING_STILL_135_DEGREES;
				break;
			case PLANT_4LEAF_ANGLE_225:
				ani = PLANT_4LEAF_ANI_RED_KEEPING_STILL_225_DEGREES;
				break;
			case PLANT_4LEAF_ANGLE_315:
				ani = PLANT_4LEAF_ANI_RED_KEEPING_STILL_315_DEGREES;
				break;
			}
		}
	}
	else if(type == PLANT_4LEAF_TYPE_GREEN)
	{

	}
	animation_set->at(ani)->Render(x, y);
}
void CPlant_4Leaf::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

}