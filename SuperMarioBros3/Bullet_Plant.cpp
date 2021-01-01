#include "Bullet_Plant.h"
#include "Mario.h"
#include "PlayScence.h"
#include "Utils.h"

#define PI 3.1415

CBullet_Plant::CBullet_Plant(float _x, float _y, int angle) :CBullet(_x, _y)
{
	state = BULLET_STATE_FLYING;
	Calculate_vxvy(angle);
}

void CBullet_Plant::Calculate_vxvy(int angle)
{
	vx = cos(angle*PI/180) * BULLET_PLANT_SPEED;
	vy = sin(angle*PI/180) * BULLET_PLANT_SPEED;
}

void CBullet_Plant::CalcPotentialCollisionWithMario()
{
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	LPCOLLISIONEVENT e = SweptAABBEx(mario);

	if (e->t > 0 && e->t <= 1.0f)
	{
		if (!mario->IsUntouchable() )
		{ 
			mario->BeDamaged();

		}
			
	}
}

void CBullet_Plant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	x += dx;
	y += dy;
	CalcPotentialCollisionWithMario();
}

CBullet_Plant::~CBullet_Plant()
{

}
