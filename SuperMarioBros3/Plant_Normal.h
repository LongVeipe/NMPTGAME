#pragma once
#include "Plant.h"

#define PLANT_NORMAL_BBOX_HEIGHT	32
#define PLANT_NORMAL_BBOX_WIDTH		16

#define PLANT_NORMAL_TYPE_RED		1
#define PLANT_NORMAL_TYPE_GREEN		2

#define PLANT_NORMAL_ANI_RED		0
#define PLANT_NORMAL_ANI_GREEN		1

#define PLANT_NORMAL_ATTACKING_TIME	1000
#define PLANT_NORMAL_SLEEPING_TIME	1500

#define PLANT_NORMAL_MOVING_SPEED_Y	0.035f

#define PLANT_NORMAL_STATE_MOVING_UP		1
#define PLANT_NORMAL_STATE_MOVING_DOWN	2
#define PLANT_NORMAL_STATE_SLEEPING		4
#define PLANT_NORMAL_STATE_ATTACKING		5


#define PLANT_NORMAL_MIN_DISTANCE_FROM_MARIO 16

class CPlant_Normal : public CPlant
{
private:
	bool IsAttacking;
	DWORD attacking_start;

	void UpdateInLoop();

public:
	CPlant_Normal(float _x, float _y, float _limit_y, int _type);
	~CPlant_Normal();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int _state);

	void SetType(int _type) { type = _type; }
	int GetType() { return type; }
	virtual bool IsUpdatable();
};

