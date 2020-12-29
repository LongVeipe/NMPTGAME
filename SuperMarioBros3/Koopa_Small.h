#pragma once
#include "Koopas.h"
#include "Wing.h"

#define KOOPA_BE_KNOCKED_DOWN_SPEED_Y 0.3f
#define KOOPA_BE_KNOCKED_DOWN_SPEED_X 0.04f
#define KOOPA_JUMP_SPEED_Y		0.28f
#define KOOPA_GRAVITY			0.001f
#define KOOPA_MAX_FALL_SPEED	0.12
#define KOOPA_WALKING_SPEED 0.033f;
#define KOOPA_SPEED_TURTOISESHELL_X	0.2

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

#define KOOPA_SMALL_ANI_GREEN_WALKING_LEFT						0
#define KOOPA_SMALL_ANI_GREEN_WALKING_RIGHT						1
#define KOOPA_SMALL_ANI_GREEN_TURTOISESHELL_IDLE				2
#define KOOPA_SMALL_ANI_GREEN_TURTOISESHELL_WAGGLE				3
#define KOOPA_SMALL_ANI_GREEN_TURTOISESHELL_RUNNING				4
#define KOOPA_SMALL_ANI_GREEN_TURTOISESHELL_BE_KNOCKED_DOWN		5
#define KOOPA_SMALL_ANI_GREEN_TURTOISESHELL_WAGGLE_2			6

#define KOOPA_SMALL_ANI_RED_WALKING_LEFT						7
#define KOOPA_SMALL_ANI_RED_WALKING_RIGHT						8
#define KOOPA_SMALL_ANI_RED_TURTOISESHELL_IDLE					9
#define KOOPA_SMALL_ANI_RED_TURTOISESHELL_WAGGLE				10
#define KOOPA_SMALL_ANI_RED_TURTOISESHELL_RUNNING				11
#define KOOPA_SMALL_ANI_RED_TURTOISESHELL_BE_KNOCKED_DOWN		12
#define KOOPA_SMALL_ANI_RED_TURTOISESHELL_WAGGLE_2				13

#define KOOPA_RED_SMALL_TURTOISESHELL_ANI_IDLE 0
#define KOOPA_RED_SMALL_ANI_DIE 2

class CKoopa_Small: public CKoopas
{
private:
	CWing* leftWing;
	CWing* rightWing;

	bool isEnable;
	bool isReadyToEnable;
public:
	bool IsBeingHeld;
	bool IsWaggling;


	CKoopa_Small(float _x, float _y, int _type);
	virtual ~CKoopa_Small();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();


	void Update_vy();
	bool CalculateTurningAround(vector<LPGAMEOBJECT>* coObjects);
	void TurnAround();
	void Update_Wings();
	void UpdateFlyingType();

	virtual void SetState(int state);
	int GetType() { return this->type; }

	void BeHeld();
	void BeKicked(int mnx);
	void CalculateBeSwingedTail();
	void BeDamaged_Y();

	virtual void Reset();
};

