#include "Bullet.h"

CBullet::CBullet(float _x, float _y): CGameObject()
{
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(BULLET_ANIMATION_SET_ID);
	this->SetAnimationSet(ani_set);

	x = _x;
	y = _y;
}
CBullet::~CBullet()
{

}
void CBullet::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == BULLET_STATE_EXPLODING)
		left = top = right = bottom = 0;
	else
	{
		left = x;
		top = y;
		right = x + BULLET_BBOX_WIDTH;

		bottom = y + BULLET_BBOX_HEIGHT;
	}
}
void CBullet::Render()
{
	//CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	int ani = -1;
	if (state == BULLET_STATE_FLYING)
	{
		if (nx > 0)
			ani = BULLET_ANI_FLYING_RIGHT;
		else
			ani = BULLET_ANI_FLYING_LEFT;
	}
	if (state == BULLET_STATE_EXPLODING)
	{
		if (nx > 0)
			ani = BULLET_ANI_EXPLODING_RIGHT;
		else
			ani = BULLET_ANI_EXPLODING_LEFT;
	}
	animation_set->at(ani)->Render(x, y);
}