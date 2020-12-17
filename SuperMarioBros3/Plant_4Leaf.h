#pragma once
#include "Plant.h"
#include "Bullet_Plant.h"

#define PLANT_4LEAF_BBOX_HEIGHT	32
#define PLANT_4LEAF_BBOX_WIDTH		16

#define PLANT_4LEAF_TYPE_RED		1
#define PLANT_4LEAF_TYPE_GREEN		2

#define PLANT_4LEAF_ANI_RED_MOVING_45_DEGREES			0
#define PLANT_4LEAF_ANI_RED_KEEPING_STILL_45_DEGREES	1
#define PLANT_4LEAF_ANI_RED_MOVING_315_DEGREES			2
#define PLANT_4LEAF_ANI_RED_KEEPING_STILL_315_DEGREES	3
#define PLANT_4LEAF_ANI_RED_MOVING_135_DEGREES			4
#define PLANT_4LEAF_ANI_RED_KEEPING_STILL_135_DEGREES	5
#define PLANT_4LEAF_ANI_RED_MOVING_225_DEGREES			6
#define PLANT_4LEAF_ANI_RED_KEEPING_STILL_225_DEGREES	7

#define PLANT_4LEAF_ANI_GREEN_MOVING_45_DEGREES			8
#define PLANT_4LEAF_ANI_GREEN_KEEPING_STILL_45_DEGREES	9
#define PLANT_4LEAF_ANI_GREEN_MOVING_315_DEGREES		10
#define PLANT_4LEAF_ANI_GREEN_KEEPING_STILL_315_DEGREES	11
#define PLANT_4LEAF_ANI_GREEN_MOVING_135_DEGREES		12
#define PLANT_4LEAF_ANI_GREEN_KEEPING_STILL_135_DEGREES	13
#define PLANT_4LEAF_ANI_GREEN_MOVING_225_DEGREES		14
#define PLANT_4LEAF_ANI_GREEN_KEEPING_STILL_225_DEGREES	15

#define PLANT_4LEAF_ANGLE_30	30
#define PLANT_4LEAF_ANGLE_45	45
#define PLANT_4LEAF_ANGLE_135	135
#define PLANT_4LEAF_ANGLE_150	150
#define PLANT_4LEAF_ANGLE_210	210
#define PLANT_4LEAF_ANGLE_225	225
#define PLANT_4LEAF_ANGLE_315	315
#define PLANT_4LEAF_ANGLE_330	330

#define PLANT_4LEAF_IS_INTERSECT_WITH_MARIO_ON_THE_LEFT		1
#define PLANT_4LEAF_IS_INTERSECT_WITH_MARIO_ON_THE_RIGHT	2
#define PLANT_4LEAF_IS_ON_MARIOS_LEFT						3
#define PLANT_4LEAF_IS_ON_MARIOS_RIGHT						4

#define PLANT_4LEAF_SHOOTING_BULLET_TIME	500
#define PLANT_4LEAF_SIGHTING_TIME	1000
#define PLANT_4LEAF_SLEEPING_TIME	1500

#define PLANT_4LEAF_MOVING_SPEED_Y	0.035

#define PLANT_4LEAF_STATE_MOVING_UP		1
#define PLANT_4LEAF_STATE_MOVING_DOWN	2
#define PLANT_4LEAF_STATE_SHOOTING		3
#define PLANT_4LEAF_STATE_SLEEPING		4
#define PLANT_4LEAF_STATE_SIGHTING		5

#define PLANT_4LEAF_MIN_DISTANCE_FROM_MARIO 16

class CPlant_4Leaf: public CPlant
{
private:
	vector<CBullet_Plant*> bullets;
	int	angle;
	bool IsShooting;
	DWORD shoot_start;
	DWORD sight_start;

	void CalculateAngle();
	int CalculatePositionInComparisonToMario();
	void UpdateInLoop();
	void ShootBullet_Plant();

public:
	CPlant_4Leaf(float _x, float _y, float _limit_y, int _type);
	~CPlant_4Leaf();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int _state);
	void SetAngle(int _angle) { angle = _angle; }
	int GetAngle() { return angle; }
	virtual bool IsUpdatable();
};

