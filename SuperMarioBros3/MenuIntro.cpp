#include "MenuIntro.h"
#include "Utils.h"

CMenuIntro::CMenuIntro()
{
	SetState(MENU_STATE_1PLAYER);
}

void CMenuIntro::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

}
void CMenuIntro::Render()
{
	if (!IsEnable)
		return;
	if (state == MENU_STATE_1PLAYER)
		CSprites::GetInstance()->Get(MENU_SPRITE_1PLAYER_ID)->Draw(x, y);
	else
		CSprites::GetInstance()->Get(MENU_SPRITE_2PLAYER_ID)->Draw(x, y);
}
void CMenuIntro::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{

}