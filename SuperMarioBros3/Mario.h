#pragma once
#include "GameObject.h"

#define MARIO_WALKING_SPEED_START	0.08f 
#define MARIO_WALKING_ACCELERATION	0.0001f
#define MARIO_WALKING_FRICTION	0.00025f
#define MARIO_WALKING_SPEED_MAX		0.13
//0.1f
#define MARIO_JUMP_SPEED_Y		0.2f
#define MARIO_JUMP_DEFLECT_SPEED 0.2f
#define MARIO_GRAVITY			0.002f
#define MARIO_DIE_DEFLECT_SPEED	 0.5f



#define MARIO_STATE_IDLE			0
#define MARIO_STATE_WALKING			100
#define MARIO_STATE_WALKING_RIGHT	100
#define MARIO_STATE_WALKING_LEFT	200
#define MARIO_STATE_JUMP			300
#define MARIO_STATE_DIE				400
#define MARIO_STATE_BEND_DOWN		500

#define MARIO_ANI_BIG_IDLE_RIGHT			0
#define MARIO_ANI_BIG_IDLE_LEFT				1
#define MARIO_ANI_BIG_WALKING_RIGHT			2
#define MARIO_ANI_BIG_WALKING_LEFT			3
#define MARIO_ANI_BIG_JUMPING_RIGHT			4
#define MARIO_ANI_BIG_JUMPING_LEFT			5
#define MARIO_ANI_BIG_RUNNING_RIGHT			6
#define MARIO_ANI_BIG_RUNNING_LEFT			7
#define MARIO_ANI_BIG_RUNJUMPING_RIGHT		8
#define MARIO_ANI_BIG_RUNJUMPING_LEFT		9
#define MARIO_ANI_BIG_HOLDING_IDLE_RIGHT	10
#define MARIO_ANI_BIG_HOLDING_RIGHT			11
#define MARIO_ANI_BIG_HOLDING_JUMP_RIGHT	12
#define MARIO_ANI_BIG_HOLDING_IDLE_LEFT		13
#define MARIO_ANI_BIG_HOLDING_LEFT			14
#define MARIO_ANI_BIG_HOLDING_JUMP_LEFT		15
#define MARIO_ANI_BIG_DUCKING_RIGHT			16
#define MARIO_ANI_BIG_DUCKING_LEFT			17
#define MARIO_ANI_BIG_KICKING_RIGHT			18
#define MARIO_ANI_BIG_KICKING_LEFT			19
#define MARIO_ANI_BIG_SKIDDING_RIGHT		20
#define MARIO_ANI_BIG_SKIDDING_LEFT			21

#define MARIO_ANI_SMALL_IDLE_RIGHT			22
#define MARIO_ANI_SMALL_IDLE_LEFT			23
#define MARIO_ANI_SMALL_WALKING_RIGHT		24
#define MARIO_ANI_SMALL_WALKING_LEFT		25
#define MARIO_ANI_SMALL_JUMPING_RIGHT		26
#define MARIO_ANI_SMALL_JUMPING_LEFT		27
#define MARIO_ANI_SMALL_SKIDDING_RIGHT		28
#define MARIO_ANI_SMALL_SKIDDING_LEFT		29
#define MARIO_ANI_SMALL_RUNNING_RIGHT		30
#define MARIO_ANI_SMALL_RUNNING_LEFT		31
#define MARIO_ANI_SMALL_RUNNING_JUMP_RIGHT	32
#define MARIO_ANI_SMALL_RUNNING_JUMP_LEFT	33
#define MARIO_ANI_SMALL_HOLDING_IDLE_RIGHT	34
#define MARIO_ANI_SMALL_HOLDING_IDLE_LEFT	35
#define MARIO_ANI_SMALL_HOLDING_RIGHT		36
#define MARIO_ANI_SMALL_HOLDING_LEFT		37
#define MARIO_ANI_SMALL_HOLDING_JUMP_RIGHT	38
#define MARIO_ANI_SMALL_HOLDING_JUMP_LEFT	39
#define MARIO_ANI_SMALL_KICKING_RIGHT		40
#define MARIO_ANI_SMALL_KICKING_LEFT		41
#define MARIO_ANI_SMALL_SLIDING_RIGHT		42
#define MARIO_ANI_SMALL_SLIDING_LEFT		43
#define MARIO_ANI_DIE						44

#define MARIO_ANI_RACCOON_IDLE_RIGHT	13
#define MARIO_ANI_RACCOON_IDLE_LEFT		14
#define MARIO_ANI_RACCOON_WALKING_RIGHT	15
#define MARIO_ANI_RACCOON_WALKING_LEFT	16
#define MARIO_ANI_RACCOON_JUMPING_RIGHT	17
#define MARIO_ANI_RACCOON_JUMPING_LEFT	18
#define MARIO_ANI_RACCOON_FALLING_RIGHT	19
#define MARIO_ANI_RACCOON_FALLING_LEFT	20
#define MARIO_ANI_RACCOON_RUNNING_RIGHT	21
#define MARIO_ANI_RACCOON_RUNGING_LEFT	22
#define MARIO_ANI_RACCOON_FLYING_RIGHT	23
#define MARIO_ANI_RACCOON_FLYPING_LEFT	24
#define MARIO_ANI_RACCOON_DUCKING_RIGHT	25
#define MARIO_ANI_RACCOON_DUCKING_LEFT	26
#define MARIO_ANI_RACCOON_SLIDING_RIGHT	25
#define MARIO_ANI_RACCOON_SLIDING_LEFT	26
#define MARIO_ANI_RACCOON_SKIDDING_RIGHT	25
#define MARIO_ANI_RACCOON_SKIDDING_LEFT	26
#define MARIO_ANI_RACCOON_HOLDING_RIGHT	25
#define MARIO_ANI_RACCOON_HOLDING_LEFT	26
#define MARIO_ANI_RACCOON_SPINNING_RIGHT	27
#define MARIO_ANI_RACCOON_SPINNING_LEFT	28


#define	MARIO_LEVEL_SMALL	1
#define	MARIO_LEVEL_BIG		2
#define	MARIO_LEVEL_RACCOON	3
#define	MARIO_MAX_LEVEL		3

#define MARIO_BIG_BBOX_WIDTH  14
#define MARIO_BIG_BBOX_HEIGHT 27
#define MARIO_SMALL_BBOX_WIDTH  13
#define MARIO_SMALL_BBOX_HEIGHT 15
#define MARIO_RACCOON_BBOX_WIDTH  21
#define MARIO_RACCOON_BBOX_HEIGHT 27

#define MARIO_UNTOUCHABLE_TIME 5000
#define MARIO_UP_IMMINENT_TIME 500
#define MARIO_MAX_JUMPIMG_STACKS 19
#define MARIO_MAX_IMMINENT_STACKS 7


class CMario : public CGameObject
{
	int level;
	int untouchable;
	DWORD untouchable_start;
	DWORD upImminent_start;

	float start_x;			// initial position of Mario at scene
	float start_y; 
	int jumpStack;
	int imminentStack;
	void Calculate_vx(DWORD _dt);
	void BasicCollision(float min_tx, float min_ty, float nx, float ny, float x0, float y0, float oleft, float otop, float oright, float obottom);
public: 
	bool IsReadyJump;
	bool IsReadyHolding;


	bool IsTouchingGround;
	bool IsJumping;
	bool IsFalling;
	bool IsFlying;
	bool IsWalkingRight;
	bool IsWalkingLeft;
	bool IsHolding;
	bool IsIdle;
	bool IsKicking;


	CMario(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	virtual void Render();

	void SetState(int state);
	void SetLevel(int l) { level = l; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }
	void SetJumpStack(int _num) { jumpStack = _num; }


	void Reset();

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	int GetJumpStack() { return jumpStack; }
	int GetLevel() { return level; }
	void UpJumpStack() { jumpStack += 1; }
	
};