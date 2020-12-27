#pragma once
#include "GameObject.h"
#include "Wing.h"

#define GOOMBA_WALKING_SPEED 0.025f
#define GOOMBA_DIE_X_SPEED_Y 0.3f
#define GOOMBA_DIE_X_SPEED_X 0.04f
#define GOOMBA_JUMP_SPEED_Y		0.15f
#define GOOMBA_FLY_SPEED_Y		0.3f
#define GOOMBA_GRAVITY			0.001f
#define GOOMBA_MAX_FALL_SPEED	0.15


#define GOOMBA_BBOX_NORMAL_WIDTH 16
#define GOOMBA_BBOX_NORMAL_HEIGHT 15
#define GOOMBA_BBOX_NORMAL_HEIGHT_DIE 9
#define GOOMBA_BBOX_RED_WIDTH 16
#define GOOMBA_BBOX_RED_HEIGHT 15
#define GOOMBA_BBOX_RED_HEIGHT_DIE 9

#define GOOMBA_TYPE_NORMAL	1
#define GOOMBA_TYPE_FLYING_RED	2
#define GOOMBA_TYPE_WALKING_RED	3

#define GOOMBA_STATE_WALKING 100
#define GOOMBA_STATE_DIE_X 200
#define GOOMBA_STATE_DIE_Y 300
#define GOOMBA_STATE_JUMPING	400
#define GOOMBA_STATE_FLYING		500

#define GOOMBA_ANI_NORMAL_WALKING	0
#define GOOMBA_ANI_NORMAL_DIE_Y	1
#define GOOMBA_ANI_NORMAL_DIE_X	2

#define GOOMBA_ANI_RED_WALKING	3
#define GOOMBA_ANI_RED_DIE_Y	4
#define GOOMBA_ANI_RED_DIE_X	5


#define GOOMBA_TIME_TO_STOP_RENDERING 200
#define GOOMBA_TIME_PARA_OPERATION_LOOP	2500

#define GOOMBA_PARA_MAX_JUMP_STACK	2


class CGoomba : public CGameObject
{
private:
	int type;
	int PARA_jumpStack;
	DWORD DeadTime;
	DWORD loop_start;

	CWing* leftWing;
	CWing* rightWing;

	void CalculateBeSwingedTail();
	void Calculate_vx();
	void Calculate_vy();
	void Update_FlyingRed();
	void Update_Wings();

public:

	CGoomba(float start_x, float final_x, int type);
	virtual void SetState(int state);
	virtual ~CGoomba();
	void SetDeadTime();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	void BeDamaged_Y();

	bool IsTouchingGround;
	bool IsSpreadingWing;

};