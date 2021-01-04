#pragma once
#include "GameObject.h"

#define SWITCH_BLOCK_BBOX_WIDTH				16
#define SWITCH_BLOCK_BBOX_INACTIVE_HEIGHT	16
#define SWITCH_BLOCK_BBOX_ACTIVE_HEIGHT		7


#define SWITCH_BLOCK_ANI_INACTIVE	0
#define SWITCH_BLOCK_ANI_ACTIVE		1

#define SWITCH_BLOCK_ANI_SET_ID			72000

class CSwitchBlock : public CGameObject
{
public:
	bool isActive;
	CSwitchBlock(float _x, float _y);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	void CalcPotentialCollisionWithMario(vector<LPGAMEOBJECT>* coObjects);
	void HideGoldBox(vector<LPGAMEOBJECT>* coObjects);
};

