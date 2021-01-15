#pragma once
#include "GameObject.h"
#include "Sprites.h"

#define MENU_SPRITE_1PLAYER_ID	20011
#define MENU_SPRITE_2PLAYER_ID	20012

#define MENU_STATE_1PLAYER		1
#define MENU_STATE_2PLAYER		2


class CMenuIntro: public CGameObject
{
public:
	CMenuIntro();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

};

