#include "Item.h"


CItem::CItem()
{
	type = ITEM_TYPE_STAR;
	changeTypeTime = 0;
	IsIntact = true;
}
void CItem::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects )
{
	if(IsIntact)
		UpdateType(dt);
	else
	{
		CGameObject::Update(dt);
		y += dy;
	}
}
void CItem::Render()
{
	int ani = -1;
	switch (type)
	{
	case ITEM_TYPE_STAR:
		ani = ITEM_ANI_STAR;
		break;
	case ITEM_TYPE_MUSHROOM:
		ani = ITEM_ANI_MUSHROOM;
		break;
	case ITEM_TYPE_FLOWER:
		ani = ITEM_ANI_FLOWER;
		break;
	}

	animation_set->at(ani)->Render(x, y);
}
void CItem::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + ITEM_BBOX_WIDTH;
	bottom = top + ITEM_BBOX_HEIGHT;
}

void CItem::UpdateType(DWORD _dt)
{
	changeTypeTime += _dt;
	if (changeTypeTime > ITEM_CHANGE_TYPE_TIME)
	{
		changeTypeTime = 0;
		switch (type)
		{
		case ITEM_TYPE_STAR:
			type = ITEM_TYPE_MUSHROOM;
			break;
		case ITEM_TYPE_MUSHROOM:
			type = ITEM_TYPE_FLOWER;
			break;
		case ITEM_TYPE_FLOWER:
			type = ITEM_TYPE_STAR;
			break;
		}
	}
}
void CItem::BeTaken()
{
	IsIntact = false;
	vy = -ITEM_SPEED_Y;
}