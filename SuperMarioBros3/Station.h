#pragma once
#include "GameObject.h"

#define STATION_BBOX_WIDTH	16
#define STATION_BBOX_HEIGHT	16

class CStation: public CGameObject
{
private:
	bool openLeft;
	bool openTop;
	bool openRight;
	bool openBottom;
	int IdPlayScene;

	void CalcPotentialPreventMarioWM();

public:
	bool IsReadyToPrevent = false;

	CStation(float x , float , bool l, bool t, bool r, bool b, int id);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void GetWay(bool& l, bool& t, bool& r, bool& b);
	void SetState(int state);

};

