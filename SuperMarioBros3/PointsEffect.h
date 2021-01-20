#pragma once
#include "Sprites.h"
#include "GameObject.h"
#include <map>
using namespace std;

#define POINTS_SPRITE_100	999201
#define POINTS_SPRITE_200	999202
#define POINTS_SPRITE_400	999203
#define POINTS_SPRITE_800	999204
#define POINTS_SPRITE_1000	999205
#define POINTS_SPRITE_2000	999206
#define POINTS_SPRITE_4000	999207
#define POINTS_SPRITE_8000	999208
#define POINTS_SPRITE_1VP	999209

#define POINTS_100		100
#define POINTS_200		200
#define POINTS_400		400
#define POINTS_800		800
#define POINTS_1000		1000
#define POINTS_2000		2000
#define POINTS_4000		4000
#define POINTS_8000		8000
#define POINTS_1VP		0

#define	POINTS_FLYING_SPEED_Y	0.08f
#define POINTS_GRAVITY			0.0001f


class CPointsEffect: public CGameObject
{
private:
	LPSPRITE sprite;

public:
	CPointsEffect(float _x, float _y, unsigned int p);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt);
	virtual void Render();

	LPSPRITE InitSprite(int point);
};

class CPointsEffects
{
	static CPointsEffects* __instance;
	vector<CPointsEffect*> pointsEffects;

public:
	void Add(CPointsEffect* pointsEffect);
	void Update(DWORD dt);
	void Render();
	void Clear();

	static CPointsEffects* GetInstance();
};