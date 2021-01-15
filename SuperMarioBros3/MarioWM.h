#pragma once
#include "GameObject.h"
#include "Station.h"

#define MARIOWM_STATE_IDLE			0
#define MARIOWM_STATE_WALKING_UP	1
#define MARIOWM_STATE_WALKING_DOWN	2
#define MARIOWM_STATE_WALKING_LEFT	3
#define MARIOWM_STATE_WALKING_RIGHT	4


#define	MARIOWM_LEVEL_SMALL	1
#define	MARIOWM_LEVEL_BIG		2
#define	MARIOWM_LEVEL_RACCOON	3
#define	MARIOWM_LEVEL_FIRE	4

#define	MARIOWM_ANI_SMALL		0
#define	MARIOWM_ANI_BIG		1
#define	MARIOWM_ANI_RACCOON	2
#define	MARIOWM_ANI_FIRE		3

#define MARIOWM_BBOX_SMALL_WIDTH	14
#define MARIOWM_BBOX_SMALL_HEIGHT	16
#define MARIOWM_BBOX_BIG_WIDTH		16
#define MARIOWM_BBOX_BIG_HEIGHT		21
#define MARIOWM_BBOX_RACCOON_WIDTH	16
#define MARIOWM_BBOX_RACCOON_HEIGHT	24
#define MARIOWM_BBOX_FIRE_WIDTH		16
#define MARIOWM_BBOX_FIRE_HEIGHT	21


#define MARIOWM_WALKING_SPEED		0.1f

#define MARIOWM_BE_PREVENTED_TIME	200

class CMarioWM: public CGameObject
{
private:
	int level;
	bool canWalkToLeft;
	bool canWalkUp;
	bool canWalkToRight;
	bool canWalkDown;
	DWORD bePrevented_start;
	int IdPlayScene;
public:
	bool IsBeingPrevented;

	CMarioWM(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void SetState(int state);
	void SetLevel(int l) { level = l; }

	void BePrevented();
};

