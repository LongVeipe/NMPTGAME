#include "NameOfGame.h"
#include "HUD.h"
#include "Utils.h"

CNameOfGame::CNameOfGame()
{
	x = 0;
	y = -NAMEOFGAME_HEIGHT;
	spriteId = NAMEOFGAME_SPRITE_BLACK;
	sprite = CSprites::GetInstance()->Get(spriteId);
	shining = CAnimations::GetInstance()->Get(NAMEOFGAME_ANI_SHINING);
	vy = NAMEOFGAME_SPEED_Y;
	ny = 1;
	isFinish = false;
	IsEnable = false;
	shake_start = (DWORD)0;
}
void CNameOfGame::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	
}
void CNameOfGame::Update(DWORD dt)
{
	if (!IsEnable)
		return;
	if (y < 0)
	{
		CGameObject::Update(dt);
		y += dy;
	}
	else
	{
		if (spriteId == NAMEOFGAME_SPRITE_BLACK)
		{
			if (isReadyToShake)
			{
				// v?a b? kéo xu?ng
				isShaking = true;
				shake_start = (DWORD)GetTickCount64();
				isReadyToShake = false;
				y = 0;
			}
			else
			{
				if (isShaking)
				{
					Shake();
					if (GetTickCount64() - shake_start > NAMEOFGAME_SHAKING_TIME)
					{
						isShaking = false;
						y = 0;
					}
				}
				else 
				{
					if (spriteId == NAMEOFGAME_SPRITE_BLACK)
					{
						spriteId = NAMEOFGAME_SPRITE_YELLOW;
						sprite = CSprites::GetInstance()->Get(spriteId);
						
					}
					else
					{
						isFinish = true;
					}
				}
			}
		}
		else
		{

		}
	}
}
void CNameOfGame::Render()
{
	if (!IsEnable)
		return;
	sprite->Draw(x, y);
	if (!isReadyToShake && !isShaking)
		shining->Render(112, 98);
}

void CNameOfGame::Shake()
{
	if (y >= 0 && y < 1 && ny >0)
		y += 0.4f;
	else if (y >= 1)
	{
		y -= 0.4f;
		ny = -1;
	}
	else if (y < 1 && y > 0 && ny < 0)
	{
		y -= 0.4f;
		if (y < 0)
			y = 0;
	}
	else if (y == 0)
	{
		ny = 1;
		y += 0.4f;
	}
}
