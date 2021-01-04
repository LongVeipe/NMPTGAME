#include "RewardBox.h"
#include "Coin.h"
#include "Reward_LevelUp.h"
#include "SwitchBlock.h"
#include "Mario.h"
#include "PlayScence.h"
#include "Utils.h"

CRewardBox::CRewardBox(float _x, float _y, int _type, int _rewardType)
{
	this->type = _type;
	this->rewardType = _rewardType;
	this->start_y = _y;

	isEmpty = false;
	isEnable = true;
	SetState(REWARD_BOX_STATE_IDLE);
}
CRewardBox::~CRewardBox()
{
	delete reward;
	reward = nullptr;
}
void CRewardBox::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (!isEnable ||isHiding)
	{
		l = t = r = b = 0;
		return;
	}
	l = x;
	t = y;
	r = x + REWARD_BOX_BBOX_WIDTH;
	b = y + REWARD_BOX_BBOX_HEIGHT;
}
void CRewardBox::Render()
{

	if (!IsInCamera() || !isEnable)
		return;

	int ani = -1;

	if (x == 2016 && y == 384 && isHiding)
		ani = -1;
	if (isEmpty)
		ani = REWARD_BOX_ANI_EMPTY;
	else
	{
		if (type == REWARD_BOX_TYPE_GOLD)
			ani = REWARD_BOX_ANI_GOLD_INTACT;
		else
			ani = REWARD_BOX_ANI_QUESTION_INTACT;
	}

	if (reward != nullptr)
	{
		if (dynamic_cast<CReward_LevelUp*>(reward))
		{
			CReward_LevelUp* rew = (CReward_LevelUp*)reward;
			if (rew->GetType() == REWARD_LEVEL_UP_TYPE_SUPER_LEAF)
			{
				if (!isHiding)
					animation_set->at(ani)->Render(x, y);
				reward->Render();
				return;
			}
			else
				reward->Render();
		}
		else
			reward->Render();
	}
	
	if(!isHiding)
		animation_set->at(ani)->Render(x, y);
}

void CRewardBox::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	//DebugOut(L"[INFO] Update at x: %f, y: %f \n", x, y);
	if (isEmpty && state != REWARD_BOX_STATE_IDLE)
	{
		CGameObject::Update(dt, coObjects);

		if (this->y < start_y - dy)
		{
			this->vy += REWARD_BOX_GRAVITY * dt;
			if (vy > REWARD_BOX_MAX_FALLING_SPEED)
				vy = REWARD_BOX_MAX_FALLING_SPEED;
			y += dy;
		}
		else
		{
			y = start_y;
			SetState(REWARD_BOX_STATE_IDLE);
		}

	}

	CalculateBeSwingedTail();
	if (type == REWARD_BOX_TYPE_GOLD)
		Update_GoldBox(coObjects);

	if (reward != nullptr)
	{
		reward->Update(dt, coObjects);
		/*if (rewardType == REWARD_BOX_TYPE_REWARD_COIN)
		{
			CCoin* coin = (CCoin*)reward;
			if (!coin->isEnable)
			{
				delete reward;
				reward = nullptr;
			}
		}*/
	}
}

void CRewardBox::Update_GoldBox(vector<LPGAMEOBJECT>* coObjects)
{
	if (rewardType == REWARD_BOX_TYPE_REWARD_SWITCH && reward != nullptr)
	{
		CSwitchBlock* s = (CSwitchBlock*)reward;
		s->CalcPotentialCollisionWithMario(coObjects);
		
	}
}

void CRewardBox::BeBroken()
{
	isEnable = false;
}

void CRewardBox::CalculateBeSwingedTail()
{
	if (isEmpty)
		return;
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (mario->IsSwingTail)
	{
		float ml, mt, mr, mb;
		mario->GetBoundingBox(ml, mt, mr, mb);
		float bl, bt, br, bb;
		this->GetBoundingBox(bl, bt, br, bb);
		ml -= MARIO_RACCOON_TAIL_BBOX_WIDTH;
		mr += MARIO_RACCOON_TAIL_BBOX_WIDTH;
		mt += MARIO_RACCOON_HEAD_BBOX_HEIGHT;
		if (bb<mt || bt>mb || ml > br || mr < bl )
			return;
		if ((bl<ml && br>ml) || (bl < mr && mr < br))
		{
			if (rewardType == REWARD_BOX_TYPE_REWARD_COIN)
				BeBroken();
			else
			{
				CreateReward();
				isEmpty = true;
				if (rewardType == REWARD_BOX_TYPE_REWARD_LEVEL_UP)
				{
					CReward_LevelUp* lvUp = (CReward_LevelUp*)reward;
					lvUp->SetState(REWARD_LEVEL_UP_STATE_JUMPING);
				}
				else if (rewardType == REWARD_BOX_TYPE_REWARD_SWITCH)
				{

				}
			}
		}
	}
}
void CRewardBox::BeCollidedOnTheBottom()
{
	if (type == REWARD_BOX_TYPE_GOLD)
	{
		if (rewardType != REWARD_BOX_TYPE_REWARD_COIN)
		{
			if (!isEmpty)
			{
				CreateReward();
				if (rewardType == REWARD_BOX_TYPE_REWARD_LEVEL_UP)
				{
					CReward_LevelUp* lvUp = (CReward_LevelUp*)reward;
					lvUp->SetState(REWARD_LEVEL_UP_STATE_JUMPING);
				}
				SetState(REWARD_BOX_STATE_JUMPING);
				isEmpty = true;
			}
		}
		else
		{
			BeBroken();
		}
	}
	else
	{
		if (!isEmpty)
		{
			CreateReward();
			if (rewardType == REWARD_BOX_TYPE_REWARD_COIN)
			{
				CCoin* coin = (CCoin*)reward;
				coin->SetState(COIN_STATE_JUMPING);
			}
			else if (rewardType == REWARD_BOX_TYPE_REWARD_LEVEL_UP)
			{
				CReward_LevelUp* lvUp = (CReward_LevelUp*)reward;
				lvUp->SetState(REWARD_LEVEL_UP_STATE_JUMPING);
			}
			SetState(REWARD_BOX_STATE_JUMPING);
			isEmpty = true;
		}
	}
}
void CRewardBox::CreateReward()
{
	switch (rewardType)
	{
	case REWARD_BOX_TYPE_REWARD_COIN:
		if(type == REWARD_BOX_TYPE_QUESTION)
			reward = new CCoin(x + 3, y - 24);
		else
			reward = new CCoin(x + 3, y);

		reward->SetAnimationSet(CAnimationSets::GetInstance()->Get(COIN_ANI_SET_ID));
		break;
	case REWARD_BOX_TYPE_REWARD_LEVEL_UP:
		reward = new CReward_LevelUp(x, y);
		break;
	case REWARD_BOX_TYPE_REWARD_LIFE_UP:
		break;
	case REWARD_BOX_TYPE_REWARD_SWITCH:
		reward = new CSwitchBlock(x, y - SWITCH_BLOCK_BBOX_INACTIVE_HEIGHT);
		break;
	}
}


void CRewardBox::SetState(int _state)
{
	CGameObject::SetState(_state);
	switch (state)
	{
	case REWARD_BOX_STATE_IDLE:
		vy = 0;
		break;
	case REWARD_BOX_STATE_JUMPING:
		vy = -REWARD_BOX_JUMP_SPEED_Y;
		break;
	}
}
