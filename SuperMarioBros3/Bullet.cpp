#include "Bullet.h"
#include "Brick.h"
#include "Mario.h"
#include "PlayScence.h"

CBullet::CBullet(float _x, float _y) :CGameObject()
{
	x = _x;
	y = _y;
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(BULLET_ANIMATION_SET_ID);
	this->SetAnimationSet(ani_set);
}



void CBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	vy += BULLET_GRAVITY * dt;
	x += dx;
	y += dy;
	
}

void CBullet::Render()
{
	//CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	int ani = -1;
	if (nx > 0)
		ani = BULLET_ANI_RIGHT;
	else
		ani = BULLET_ANI_LEFT;
	animation_set->at(ani)->Render(x, y);
}

void CBullet::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BULLET_BBOX_WIDTH;

	bottom = y + BULLET_BBOX_HEIGHT;
}
CBullet::~CBullet()
{

}

