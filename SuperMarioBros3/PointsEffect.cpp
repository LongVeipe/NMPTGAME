#include "PointsEffect.h"
#include "HUD.h"

CPointsEffect::CPointsEffect(float _x, float _y, unsigned int point)
{
	x = _x;
	y = _y;
	vy = -POINTS_FLYING_SPEED_Y;
	sprite = InitSprite(point);
}

LPSPRITE CPointsEffect::InitSprite(int point)
{
	LPSPRITE spr = nullptr;
	CSprites* sprites = CSprites::GetInstance();
	switch (point)
	{
	case POINTS_100:
		spr = sprites->Get(POINTS_SPRITE_100);
		break;
	case POINTS_200:
		spr = sprites->Get(POINTS_SPRITE_200);
		break;
	case POINTS_400:
		spr = sprites->Get(POINTS_SPRITE_400);
		break;
	case POINTS_800:
		spr = sprites->Get(POINTS_SPRITE_800);
		break;
	case POINTS_1000:
		spr = sprites->Get(POINTS_SPRITE_1000);
		break;
	case POINTS_2000:
		spr = sprites->Get(POINTS_SPRITE_2000);
		break;
	case POINTS_4000:
		spr = sprites->Get(POINTS_SPRITE_4000);
		break;
	case POINTS_8000:
		spr = sprites->Get(POINTS_SPRITE_8000);
		break;
	case POINTS_1VP:
		spr = sprites->Get(POINTS_SPRITE_1VP);
		break;
	}

	return spr;
}

void CPointsEffect::Update(DWORD dt)
{
	CGameObject::Update(dt);
	vy += POINTS_GRAVITY*dt;
	if (vy > 0)
		vy = 0;
	y += dy;
}
void CPointsEffect::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = top = right = bottom = 0;
}
void CPointsEffect::Render()
{
	sprite->Draw(x, y - HUD_HEIGHT);
}


CPointsEffects* CPointsEffects::__instance = nullptr;

void CPointsEffects::Update(DWORD dt)
{
	for (unsigned int i = 0; i < pointsEffects.size(); i++)
	{
		CPointsEffect* pe = pointsEffects.at(i);
		if (pointsEffects.at(i)->vy == 0)
		{

			pointsEffects.erase(pointsEffects.begin() + i);
			delete pe;
		}
		else
			pe->Update(dt);
	}
}
void CPointsEffects::Render()
{
	for (unsigned int i = 0; i < pointsEffects.size(); i++)
	{
		pointsEffects.at(i)->Render();
	}
}
void CPointsEffects::Add(CPointsEffect* pointsEffect)
{
	pointsEffects.push_back(pointsEffect);
}
CPointsEffects* CPointsEffects::GetInstance()
{
	if (__instance == nullptr)
		__instance = new CPointsEffects();
	return __instance;
}
void CPointsEffects::Clear()
{
	for (unsigned int i = 0; i < pointsEffects.size(); i++)
	{
		CPointsEffect* pe = pointsEffects.at(i);
		delete pe;
		pe = nullptr;
	}
	pointsEffects.clear();
}