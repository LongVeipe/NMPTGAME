#include "Brick.h"


CBrick::CBrick(float _x, float _y, int _type)
{
	this->type = _type;
}

void CBrick::Render()
{
	int ani = 0;
	animation_set->at(ani)->Render(x, y);
}

void CBrick::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + BRICK_BBOX_WIDTH;
	b = y + BRICK_BBOX_HEIGHT;
}

