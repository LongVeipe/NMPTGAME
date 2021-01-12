#include "IntroSceneBackground.h"

CIntroSceneBackground::CIntroSceneBackground()
{
	x = 0;
	y = 0;
	nameOfGame = nullptr;
}

void CIntroSceneBackground::InitBackground()
{
	ground = CSprites::GetInstance()->Get(GROUND_SPRITE_ID);
	nameOfGame = new CNameOfGame();
	nameOfGame->IsEnable = false;
}
void CIntroSceneBackground::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (nameOfGame != nullptr)
	{
		nameOfGame->Update(dt);
	}
}

void CIntroSceneBackground::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{

}

void CIntroSceneBackground::Render()
{
	ground->Draw(0, CURTAIN_HEIGHT);
	if (nameOfGame != nullptr)
		nameOfGame->Render();
}

