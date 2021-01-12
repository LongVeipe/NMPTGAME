#pragma once
#include "GameObject.h"
#include "Sprites.h"
#include "Animations.h"

#define NAMEOFGAME_SPRITE_BLACK		20003
#define NAMEOFGAME_SPRITE_YELLOW	20004
#define NAMEOFGAME_ANI_SHINING		20001

#define NAMEOFGAME_HEIGHT		187

#define NAMEOFGAME_SPEED_Y		0.4

#define NAMEOFGAME_SHAKING_TIME	1000
class CNameOfGame: public CGameObject
{
private:
	LPSPRITE sprite;
	int spriteId;
	LPANIMATION shining;
	bool isShaking = false;
	DWORD shake_start;
	bool isReadyToShake = true;

	void Shake();
public:
	bool isFinish;

	CNameOfGame();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt);
	virtual void Render();

	int GetSpriteId() { return spriteId; }
};

