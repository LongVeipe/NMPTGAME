#include "Bullet.h"
#include "Brick.h"
#include "Mario.h"
#include "PlayScence.h"
#include "Utils.h"

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
	if (vy > BULLET_MAX_FALLING_SPEED)
		vy = BULLET_MAX_FALLING_SPEED;
	x += dx;
	y += dy;


	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;

	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		float x0 = x, y0 = y;
		x = x0 + dx;
		y = y0 + dy;
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<CBrick*>(e->obj))
			{
				if (e->nx != 0)
				{
					/*this->x = x0 + min_tx * this->dx + e->nx * 0.1f;
					this->vx = -vx;*/
					float bleft, btop, bright, bbottom;
					GetBoundingBox(bleft, btop, bright, bbottom);
					float oleft, otop, obottom, oright;
					e->obj->GetBoundingBox(oleft, otop, oright, obottom);
					if (ceil(bbottom) == otop)
					{
						this->vy = -BULLET_DEFLECT_SPEED;
						this->y = y0 + min_ty * this->dy + e->ny * 0.1f;
					}
				}
				if (e->ny != 0)
				{
					this->vy = - BULLET_DEFLECT_SPEED;
					this->y = y0 + min_ty * this->dy + e->ny * 0.1f;

						
				}
			}

		}

	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
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

