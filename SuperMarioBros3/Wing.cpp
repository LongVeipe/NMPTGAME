#include "Wing.h"
#include "Utils.h"

CWing::CWing(int _type)
{
	type = _type;
	SetState(WING_STATE_FLYING);
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(WING_ANI_SET_ID);
	this->SetAnimationSet(ani_set);
}

CWing::~CWing()
{

}

void CWing::UpdateWhenFlying()
{
	int stage = ((GetTickCount64() - fly_start) / WING_EACH_STAGE_IN_FLYING_TIME)%2;
	switch (stage)
	{
	case 0:
		if (StageOfFlying == 0)
		{
			StageOfFlying=1;
			y -= 5;
		}
		break;
	case 1:
		if (StageOfFlying == 1)
		{
			StageOfFlying = 0;
			y += 5;
		}
		break;
	}
}
void CWing::Render()
{

	int ani = -1;
	if (state == WING_STATE_FLYING)
	{
		if (type == WING_TYPE_LEFT)
			ani = WING_ANI_FLY_LEFT;
		else
			ani = WING_ANI_FLY_RIGHT;
	}
	else if (state == WING_STATE_IDLE)
	{
		if (type == WING_TYPE_LEFT)
			ani = WING_ANI_IDLE_LEFT;
		else
			ani = WING_ANI_IDLE_RIGHT;
	}
	animation_set->at(ani)->Render(x, y);
}
void CWing::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{

}

void CWing::SetState(int state)
{
	CGameObject::SetState(state);
	if (state == WING_STATE_FLYING)
	{
		StageOfFlying = 1;
		fly_start = GetTickCount64();
	}
}

