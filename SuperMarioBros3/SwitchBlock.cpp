#include "SwitchBlock.h"
#include "Mario.h"
#include "PlayScence.h"
#include "RewardBox.h"
CSwitchBlock::CSwitchBlock(float _x, float _y)
{
	x = _x;
	y = _y;
	SetAnimationSet(CAnimationSets::GetInstance()->Get(SWITCH_BLOCK_ANI_SET_ID));
	isActive = false;
}
void CSwitchBlock::Render()
{
	int ani = -1;
	if (isActive)
		ani = SWITCH_BLOCK_ANI_ACTIVE;
	else
		ani = SWITCH_BLOCK_ANI_INACTIVE;
	animation_set->at(ani)->Render(x,y);
}

void CSwitchBlock::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = l + SWITCH_BLOCK_BBOX_WIDTH;
	if (isActive)
		b = t + SWITCH_BLOCK_BBOX_ACTIVE_HEIGHT;
	else
		b = t + SWITCH_BLOCK_BBOX_INACTIVE_HEIGHT;
}
void CSwitchBlock::CalcPotentialCollisionWithMario(vector<LPGAMEOBJECT>* coObjects)
{
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	LPCOLLISIONEVENT e = SweptAABBEx(mario);

	if (e->t > 0 && e->t <= 1.0f)
	{
		float mleft, mtop, mright, mbottom;
		GetBoundingBox(mleft, mtop, mright, mbottom);
		float sleft, stop, sbottom, sright;
		e->obj->GetBoundingBox(sleft, stop, sright, sbottom);
		if (e->nx != 0)
		{
			mario->BasicCollision(e->t, e->t, -e->nx, -e->ny, mario->x, mario->y);
		}
		else if (e->ny > 0)
		{
			if (ceil(mleft) == sright || ceil(mright) == sleft)
				return;
			else
			{
				mario->BasicCollision(e->t, e->t, -e->nx, -e->ny, mario->x, mario->y);
				if (!isActive)
				{
					isActive = true;
					y = y + SWITCH_BLOCK_BBOX_INACTIVE_HEIGHT - SWITCH_BLOCK_BBOX_ACTIVE_HEIGHT;
					HideGoldBox(coObjects);
				}
			}
		}
	}
}
void CSwitchBlock::HideGoldBox(vector<LPGAMEOBJECT>* coObjects)
{
	if (isActive)
	{
		for (size_t i = 1; i < coObjects->size(); i++)
		{
			if (!coObjects->at(i)->IsInCamera())
				continue;
			if (dynamic_cast<CRewardBox*>(coObjects->at(i)))
			{
				CRewardBox* box = dynamic_cast<CRewardBox*>(coObjects->at(i));
				if (box->isEnable && box->GetRewardType() == REWARD_BOX_TYPE_REWARD_COIN)
				{

					box->isHiding = true;
					box->CreateReward();
				}
			}
		}

	}
}