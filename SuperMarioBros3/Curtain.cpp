#include "Curtain.h"
#include "IntroScene.h"
#include "Game.h"

CCurtain::CCurtain()
{
	sprite = CSprites::GetInstance()->Get(CURTAIN_SPRITE_ID);
	vy = -CURTAIN_SPEED_Y;
	isFinish = false;
}

void CCurtain::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (!IsEnable)
		return;

	CGameObject::Update(dt);
	y += dy;
	if (y < -CURTAIN_HEIGHT / 2)
	{
		CIntroScene* s = ((CIntroScene*)CGame::GetInstance()->GetCurrentScene());
		s->ShowMario();
		s->ShowLuigi();
	}
	if (y < -CURTAIN_HEIGHT)
		IsEnable = false;
}
void CCurtain::Render()
{
	if (!IsEnable)
		return;
	sprite->Draw(x, y);
}
void CCurtain::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = top = right = bottom = 0;
}
