#include "BrokenBrickEffect.h"
CBrokenBrickEffect::CBrokenBrickEffect(float _x, float _y)
{
	this->x = _x;
	this->y = _y;
	x1 = x2 = x;
	y1 = y2 = y;

	CSprites* sprites = CSprites::GetInstance();
	for (unsigned int i = 0; i < 4; i++)
	{
		brokenBrickSprite.push_back(sprites->Get(BROKEN_BRICK_EFFECT_SPRITE_ID));
	}

	this->IsEnable = true;
	StartAppear();

}

void CBrokenBrickEffect::StartAppear()
{
	appear_start = (DWORD)GetTickCount64();
	IsAppearing = true;

	vx = BROKEN_BRICK_EFFECT_SPEED_X;
	vy1 = -BROKEN_BRICK_EFFECT_SPEED_Y1;
	vy2 = -BROKEN_BRICK_EFFECT_SPEED_Y2;
}
void CBrokenBrickEffect::Render()
{
	if (!IsAppearing)
		return;

	brokenBrickSprite.at(0)->Draw(round(x1), round(y1));
	brokenBrickSprite.at(1)->Draw(round(x1), round(y2));
	brokenBrickSprite.at(2)->Draw(round(x2), round(y1));
	brokenBrickSprite.at(3)->Draw(round(x2), round(y2));
}
void CBrokenBrickEffect::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = top = right = bottom = 0;
}
void CBrokenBrickEffect::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (!IsAppearing)
		return;
	CGameObject::Update(dt);
	float dy1, dy2;
	dy1 = vy1 * dt;
	dy2 = vy2 * dt;

	x1 += dx;
	x2 += -dx;
	y1 += dy1;
	y2 += dy2;


	vy1 += dt * BRICK_FRAGMENT_GRAVITY;
	vy2 += dt * BRICK_FRAGMENT_GRAVITY;


	if (GetTickCount64() - appear_start > BROKEN_BRICK_EFFECT_APPEAR_TIME && IsAppearing)
		IsAppearing = false;
}
