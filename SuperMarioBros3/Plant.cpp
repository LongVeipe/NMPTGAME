#include "Plant.h"

CPlant::CPlant(float _x, float _y, float _limit_y, int _type)
{
	x = _x;
	y = _y;
	start_y = _y;
	limit_y = _limit_y;
	type = _type;
}

CPlant::~CPlant()
{
	
}

void CPlant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
}
