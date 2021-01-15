#include "Station.h"
#include "MarioWM.h"
#include "WorldMap.h"
#include "Game.h"

CStation::CStation(float _x, float _y, bool l, bool t, bool r, bool b, int id): CGameObject::CGameObject()
{
	x = _x;
	y = _y;
	openLeft = l;
	openTop = t;
	openRight = r;
	openBottom = b;
	IdPlayScene = id;
}
void CStation::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CalcPotentialPreventMarioWM();
	
}
void CStation::Render()
{
	RenderBoundingBox();
}
void CStation::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + STATION_BBOX_WIDTH;
	bottom = top + STATION_BBOX_HEIGHT;
}

void CStation::GetWay(bool& l, bool& t, bool& r, bool& b)
{
	l = openLeft;
	r = openRight;
	t = openTop;
	b = openBottom;
}
void CStation::SetState(int _state)
{

}

void CStation::CalcPotentialPreventMarioWM()
{
	if (!IsReadyToPrevent)
		return;
	CWorldMap* w = (CWorldMap*)CGame::GetInstance()->GetCurrentScene();
	CMarioWM* mario = w->GetPlayer();

	float ml, mt, mr, mb;
	mario->GetBoundingBox(ml, mt, mr, mb);
	float mMid = (ml + mr) / 2;
	
	float sl, st, sr, sb;
	GetBoundingBox(sl, st, sr, sb);
	float sMid = (sl + sr) / 2;

	switch (mario->GetState())
	{
	case MARIOWM_STATE_WALKING_UP:
		if (mb < sb)
		{
			mario->SetState(MARIOWM_STATE_IDLE);
			IsReadyToPrevent = false;
		}
		break;
	case MARIOWM_STATE_WALKING_DOWN:
		if (mb > sb)
		{
			mario->SetState(MARIOWM_STATE_IDLE);
			IsReadyToPrevent = false;
		}
		break;
	case MARIOWM_STATE_WALKING_LEFT:
		if (mMid < sMid)
		{
			mario->SetState(MARIOWM_STATE_IDLE);
			IsReadyToPrevent = false;
		}
		break;
	case MARIOWM_STATE_WALKING_RIGHT:
		if (mMid > sMid)
		{
			mario->SetState(MARIOWM_STATE_IDLE);
			IsReadyToPrevent = false;
		}
		break;
	}
}
