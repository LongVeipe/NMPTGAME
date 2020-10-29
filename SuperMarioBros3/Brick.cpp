#include "Brick.h"


CBrick::CBrick(int _detailType )
{
	this->DetailType = _detailType;
	if (_detailType == BRICK_TYPE_QUESTION)
		this->state = BRICK_STATE_QUESTION_INTACT;
}

void CBrick::Render()
{
	int ani = 0;
	if (DetailType == BRICK_TYPE_QUESTION)
	{
		if (state == BRICK_STATE_QUESTION_INTACT)
			ani = BRICK_ANI_QUESTION_INTACT;
		else
			ani = BRICK_ANI_QUESTION_EMPTY;
	}
	animation_set->at(ani)->Render(x, y);
	//RenderBoundingBox();
}

void CBrick::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + BRICK_BBOX_WIDTH;
	b = y + BRICK_BBOX_HEIGHT;
}
