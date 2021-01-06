#include "HUD.h"
#include "Game.h"
#include <string>
#include "PlayScence.h"

CHUD::CHUD()
{
	player = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	idWorld = 1;
	typePlayer = player->GetType();
	money = 0;
	points = 0;
	remainTime = COUNT_DOWN_TIME_DEFAULT/1000;
}
CHUD::~CHUD()
{

}

void CHUD::Render()
{
	CFont* font = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetFont();
	if(font == nullptr)
		CFont* font = new CFont();
	CSprites* sprites = CSprites::GetInstance();
	RenderMainBoard(sprites);
	RenderPlayerIcon(sprites);
	RenderImminent(sprites);
	RenderPoints(sprites, font);
	RenderMoney(sprites, font);
	RenderTime(sprites, font);
}
void CHUD::RenderMainBoard(CSprites* sprites)
{
	sprites->Get(HUD_SPRITE_MAIN_BOARD)->Draw(x, y);
}
void CHUD::RenderPlayerIcon(CSprites* sprites)
{
	if (typePlayer == MARIO)
		sprites->Get(HUD_SPRITE_MARIO_ICON)->Draw(x + HUD_PLAYER_ICON_X, y + HUD_PLAYER_ICON_Y);
	else
		sprites->Get(HUD_SPRITE_LUIGI_ICON)->Draw(x + HUD_PLAYER_ICON_X, y + HUD_PLAYER_ICON_Y);
}
void CHUD::RenderWorldId()
{

}
void CHUD::RenderImminent(CSprites* sprites)
{
	for (int i = 0; i < MARIO_MAX_IMMINENT_STACKS; i++)
	{
		if (i != MARIO_MAX_IMMINENT_STACKS - 1)
		{
			if (i < Imminent)
				sprites->Get(HUD_SPRITE_ACTIVE_NORMAL_IMMINENT)->Draw(x + HUD_IMMINENT_X + i * HUD_NORMAL_IMMINENT_WIDTH, y + HUD_IMMINENT_Y);
			else
				sprites->Get(HUD_SPRITE_INACTIVE_NORMAL_IMMINENT)->Draw(x + HUD_IMMINENT_X + i * HUD_NORMAL_IMMINENT_WIDTH, y + HUD_IMMINENT_Y);
		}
		else
		{
			if (i < Imminent)
				sprites->Get(HUD_SPRITE_ACTIVE_LAST_IMMINENT)->Draw(x + HUD_IMMINENT_X + i * HUD_NORMAL_IMMINENT_WIDTH, y + HUD_IMMINENT_Y);
			else
				sprites->Get(HUD_SPRITE_INACTIVE_LAST_IMMINENT)->Draw(x + HUD_IMMINENT_X + i * HUD_NORMAL_IMMINENT_WIDTH, y + HUD_IMMINENT_Y);
		}
	}
}
void CHUD::RenderPoints(CSprites* sprites, CFont* font)
{
	string strPoints = to_string(points);
	vector<LPSPRITE> spritesPoints = font->StringToSprites(strPoints);
	for (unsigned int i = 0; i < HUD_MAX_POINTS_NUMBER_OF_DIGIT; i++)
	{
		if (i < HUD_MAX_POINTS_NUMBER_OF_DIGIT - strPoints.size())
		{
			LPSPRITE font_0 = font->mapping('0');
			font_0->Draw(x + HUD_POINTS_X + i * FONT_WIDTH, y + HUD_POINTS_Y);
		}
		else
		{
			spritesPoints.at(i - (HUD_MAX_POINTS_NUMBER_OF_DIGIT - strPoints.size()))->Draw(x + HUD_POINTS_X + i * FONT_WIDTH, y + HUD_POINTS_Y);
		}
	}
}
void CHUD::RenderMoney(CSprites* sprites, CFont* font)
{
	string strMoney = to_string(money);
	vector<LPSPRITE> spritesMoney = font->StringToSprites(strMoney);
	for (int i = 1; i <= strMoney.size(); i++)
	{
		spritesMoney.at(strMoney.size() - i)->Draw(x + HUD_MONEY_X - i * FONT_WIDTH, y + HUD_MONEY_Y);
	}
}
void CHUD::RenderTime(CSprites* sprites, CFont* font)
{
	string strTime = to_string(remainTime);
	vector<LPSPRITE> spritesTime = font->StringToSprites(strTime);
	for (unsigned int i = 0; i < HUD_MAX_TIME_NUMBER_OF_DIGIT; i++)
	{
		if (i < HUD_MAX_TIME_NUMBER_OF_DIGIT - strTime.size())
		{
			LPSPRITE font_0 = font->mapping('0');
			font_0->Draw(x + HUD_TIME_X + i * FONT_WIDTH, y + HUD_TIME_Y);
		}
		else
		{
			spritesTime.at(i - (HUD_MAX_TIME_NUMBER_OF_DIGIT - strTime.size()))->Draw(x + HUD_TIME_X + i * FONT_WIDTH, y + HUD_TIME_Y);
		}
	}
}
void CHUD::Update(DWORD dt)
{
	Imminent = player->GetImminentStack();
	money = player->GetMoney();
	points = player->GetPoints();
	remainTime = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetRemainTime()/1000;
	if (remainTime < 0)
		remainTime = 0;

	//update x,y
	float cx, cy;
	CGame::GetInstance()->GetCamPos(cx, cy);
	x = cx;
	y = cy + CGame::GetInstance()->GetScreenHeight() - HUD_HEIGHT;
}

void CHUD::Reset()
{

}

