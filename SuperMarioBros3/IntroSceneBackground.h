#pragma once
#include "Sprites.h"
#include "Curtain.h"
#include "NameOfGame.h"
#include "GameObject.h"

#define BACKGROUND_SPRITE_ID	20001
#define GROUND_SPRITE_ID	20002

#define BACKGROUND_SECTION_UNKNOWN		-1
#define BACKGROUND_SECTION_CURTAIN		1
#define BACKGROUND_SECTION_NAMEOFGAME	2


class CIntroSceneBackground: public CGameObject
{
private:
	LPSPRITE ground;
	CNameOfGame* nameOfGame;
public:
	CIntroSceneBackground();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	void InitBackground();

	CNameOfGame* GetNameOfGame() { return nameOfGame; }
};