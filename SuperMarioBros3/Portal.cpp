#include "Portal.h"


CPortal::CPortal(float l, float t, float r, float b, int scene_id )
{
	this->scene_id = scene_id;
	x = l; 
	y = t;
	width = r - l ;
	height = b - t;
}

void CPortal::Render()
{
	//animation_set->at(0)->Render(x, y);
	RenderBoundingBox();
}

void CPortal::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x ;
	t = y -0.1;
	r = x + width;
	b = y + height;
}