#pragma once
#include "Plant.h"
#include "Bullet.h"

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

#define PLANT_4LEAF_ANGLE_45	1
#define PLANT_4LEAF_ANGLE_135	2
#define PLANT_4LEAF_ANGLE_225	3
#define PLANT_4LEAF_ANGLE_315	4

class CPlant_4Leaf: public CPlant
{
private:
	CBullet* bullet;
	int	angle;
	bool IsMoving;
public:
	CPlant_4Leaf(float _x, float _y, float _limit_y, int _type);
	~CPlant_4Leaf();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void SetAngle(int _angle) { angle = _angle; }
	int GetAngle() { return angle; }
};

