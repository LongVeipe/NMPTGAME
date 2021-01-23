#pragma once
#include"GameObject.h"

#define MOVING_EDGE_STATE_MOVING	1
#define MOVING_EDGE_STATE_STOPPING	2

#define MOVING_EDGE_SPEED		0.025f


class CMovingEdge : public CGameObject
{
	float limit_x;
public:
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render() {};
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void SetState(int state);
	CMovingEdge(float x, float y, float limit_x);

};