#pragma once
#include "GameObject.h"


#define WING_ANI_FLY_RIGHT	0
#define WING_ANI_FLY_LEFT	1
#define WING_ANI_IDLE_RIGHT	2
#define WING_ANI_IDLE_LEFT	3

#define WING_TYPE_LEFT	1
#define WING_TYPE_RIGHT	2

#define WING_STATE_FLYING	1
#define WING_STATE_IDLE		2

#define WING_ANI_SET_ID	35000

#define WING_EACH_STAGE_IN_FLYING_TIME	200

class CWing: public CGameObject
{
private:
	int type;
	DWORD fly_start;
	int StageOfFlying;
public:
	CWing(int _type);
	~CWing();
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void SetState(int _State);
	void UpdateWhenFlying();
};

