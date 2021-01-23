#pragma once
#include "Koopas.h"
#include "Wing.h"
#include "Mario.h"

#define KOOPA_BE_KNOCKED_DOWN_SPEED_Y 0.3f
#define KOOPA_BE_KNOCKED_DOWN_SPEED_X 0.04f
#define KOOPA_JUMP_SPEED_Y		0.28f
#define KOOPA_GRAVITY			0.001f
#define KOOPA_MAX_FALL_SPEED	0.12f
#define KOOPA_WALKING_SPEED 0.032f;
#define KOOPA_SPEED_TURTOISESHELL_X	0.2f
#define KOOPA_SPEED_TURTOISESHELL_DEFLECT_X	0.08f
#define KOOPA_SPEED_TURTOISESHELL_DEFLECT_Y	0.1f
#define KOOPA_RED_SMALL_FLYING_JET_SPEED	0.0002f
#define KOOPA_RED_SMALL_FLYING_MAIN_SPEED	0.05f

#define KOOPA_SMALL_BBOX_WIDTH 16
#define KOOPA_SMALL_BBOX_HEIGHT 26
#define KOOPA_SMALL_TURTOISESHELL_BBOX_WIDTH 16
#define KOOPA_SMALL_TURTOISESHELL_BBOX_HEIGHT 16


#define KOOPA_SMALL_TYPE_RED_TURTOISESHELL 11
#define KOOPA_SMALL_TYPE_RED_WALKING 12
#define KOOPA_SMALL_TYPE_RED_FLYING 13
#define KOOPA_SMALL_TYPE_GREEN_TURTOISESHELL 21
#define KOOPA_SMALL_TYPE_GREEN_WALKING 22
#define KOOPA_SMALL_TYPE_GREEN_FLYING 23

#define KOOPA_SMALL_STATE_IDLE				0
#define KOOPA_SMALL_STATE_WALKING_RIGHT		100
#define KOOPA_SMALL_STATE_WALKING_LEFT		200
#define KOOPA_SMALL_STATE_JUMPING_RIGHT		300
#define KOOPA_SMALL_STATE_JUMPING_LEFT		400
#define KOOPA_SMALL_STATE_RUNNING_RIGHT		500
#define KOOPA_SMALL_STATE_RUNNING_LEFT		600
#define KOOPA_SMALL_STATE_BE_KNOCKED_DOWN	700
#define KOOPA_SMALL_STATE_DIE				800
#define KOOPA_SMALL_STATE_FLYING_UP			900
#define KOOPA_SMALL_STATE_FLYING_DOWN		1000

#define KOOPA_SMALL_ANI_GREEN_WALKING_LEFT						0
#define KOOPA_SMALL_ANI_GREEN_WALKING_RIGHT						1
#define KOOPA_SMALL_ANI_GREEN_TURTOISESHELL_IDLE				2
#define KOOPA_SMALL_ANI_GREEN_TURTOISESHELL_WAGGLE				3
#define KOOPA_SMALL_ANI_GREEN_TURTOISESHELL_RUNNING				4
#define KOOPA_SMALL_ANI_GREEN_BE_KNOCKED_DOWN_IDLE				5
#define KOOPA_SMALL_ANI_GREEN_BE_KNOCKED_DOWN_WAGGLE			6
#define KOOPA_SMALL_ANI_GREEN_BE_KNOCKED_DOWN_RUNNING			7


#define KOOPA_SMALL_ANI_RED_WALKING_LEFT						8
#define KOOPA_SMALL_ANI_RED_WALKING_RIGHT						9
#define KOOPA_SMALL_ANI_RED_TURTOISESHELL_IDLE					10
#define KOOPA_SMALL_ANI_RED_TURTOISESHELL_WAGGLE				11
#define KOOPA_SMALL_ANI_RED_TURTOISESHELL_RUNNING				12
#define KOOPA_SMALL_ANI_RED_BE_KNOCKED_DOWN_IDLE				13
#define KOOPA_SMALL_ANI_RED_BE_KNOCKED_DOWN_WAGGLE				14
#define KOOPA_SMALL_ANI_RED_BE_KNOCKED_DOWN_RUNNING				15


#define KOOPA_SMALL_IS_TURTOISESHELL_TIME		9000
#define KOOPA_SMALL_WAGGLE_TIME				2500

class CKoopa_Small: public CKoopas
{
private:
	CWing* leftWing;
	CWing* rightWing;
	CMario* holder;

	DWORD isTurtoiseshell_start;
	//DWORD waggle_start;

public:

	bool IsBeingKnockedDown;
	bool IsBeingHeld;
	bool IsWaggling;


	CKoopa_Small(float _x, float _y, int _type);
	virtual ~CKoopa_Small();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();


	void Update_vy();
	void UpdateFlag();
	bool CalculateTurningAround(vector<LPGAMEOBJECT>* coObjects);
	void CalculateBeAtackedByBox(vector<LPGAMEOBJECT>* coObjects);
	void TurnAround();
	void Update_Wings();
	void UpdateGreenFlying();
	void UpdateRedFlying();

	virtual void SetState(int state);
	void SetHolder(CMario* _holder) { holder = _holder; }
	int GetType() { return this->type; }
	void SetType(int _type);

	void BeHeld();
	void BeKicked(int mnx);
	void BeSwingTail(CMario*);
	void CalculateBeSwingedTail();
	void BeDamaged_Y();

	virtual void Reset();

	bool IsOnTheLeftOfMario();
};

