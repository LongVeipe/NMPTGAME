#pragma once

#include "GameObject.h"

#define KOOPAS_WALKING_SPEED 0.03f;
#define KOOPA_SPEED_TURTOISESHELL_X	0.1

#define KOOPA_BBOX_RED_SMALL_WALK_WIDTH 16
#define KOOPA_BBOX_RED_SMALL_WALK_HEIGHT 27
#define KOOPA_BBOX_RED_SMALL_TURTOISESHELL_WIDTH 16
#define KOOPA_BBOX_RED_SMALL_TURTOISESHELL_HEIGHT 16
#define KOOPAS_BBOX_HEIGHT_DIE 16


#define KOOPA_TYPE_RED_SMALL_TURTOISESHELL 10

#define KOOPA_STATE_WALKING 100
#define KOOPA_STATE_IDLE 0
#define KOOPA_STATE_DIE 200

#define KOOPA_ANI_RED_SMALL_TURTOISESHELL_IDLE 0
#define KOOPAS_ANI_DIE 2

#define KOOPA_GRAVITY 0.002f

class CKoopas : public CGameObject
{
private:
	int type;

public:
	bool IsBeingHeld;

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	CKoopas(float _start_x, float _final_x, int _type);
	virtual void SetState(int state);
	int GetType() { return this->type; }
	virtual ~CKoopas();
	void BeHeld();
	void BeKicked(int _mvx);
};