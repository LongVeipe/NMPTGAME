#include "Portal.h"


CPortal::CPortal(float l, float t, float r, float b, int _targetZone, float _targetX, float _targetY, int _type)
{
	this->targetZone = _targetZone;
	this->targetX = _targetX;
	this->targetY = _targetY;
	x = l; 
	y = t;
	width = r - l ;
	height = b - t;
	type = _type;
}

void CPortal::Render()
{
	//animation_set->at(0)->Render(x, y);
	RenderBoundingBox();
}

void CPortal::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x ;
	t = y -0.1f;
	r = x + width;
	b = y + height;
}