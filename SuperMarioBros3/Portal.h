#pragma once

#include "GameObject.h"

#define PORTAL_TYPE_ACTIVE	1
#define PORTAL_TYPE_PASSIVE	2

class CPortal : public CGameObject
{
	int targetZone;	// target scene to switch to 

	float targetX;
	float targetY;

	float width;
	float height;

	int type;
public:
	CPortal(float l, float t, float r, float b, int _targetZone, float targetX, float targetY, int type);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	int GetTartgetZone() { return targetZone; }
	float GetTargetX() { return targetX; }
	float GetTargetY() { return targetY; }
	int GetType() { return type; }
};