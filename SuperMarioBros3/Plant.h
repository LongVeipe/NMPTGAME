#pragma once
#include "GameObject.h"


#define PLANT_TYPE_RED_2_LEAF		3
#define PLANT_TYPE_GREEN_2_LEAF		4

const double PI = 3.141592653589793;

class CPlant: public CGameObject
{
protected:
	int type;
	float start_y;
	float limit_y;
	bool IsSleeping;
	bool IsMoving;

	DWORD sleep_start;
public:

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) = 0;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render() = 0;
	CPlant(float x, float y, float _limit_y, int _type);
	virtual ~CPlant ();
	int GetType() { return this->type; }

};

