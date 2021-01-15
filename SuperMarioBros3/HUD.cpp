#include "HUD.h"
#include "Game.h"
#include <string>
#include "PlayScence.h"
#include "WorldMap.h"
#include "Item.h"

CHUD::CHUD(int _typeS)
{
	font = new CFont();
	typeScene = _typeS;
	CScene* s = CGame::GetInstance()->GetCurrentScene();
	if (typeScene == HUD_TYPE_PLAYSCENE)
	{
		mario = ((CPlayScene*)s)->GetPlayer();
		typePlayer = mario->GetType();
		typeCard = mario->GetTypeCard();
	}
	else
	{
		marioWM = ((CWorldMap*)s)->GetPlayer();
		typePlayer =((CMarioWM*) marioWM)->GetType();
		typeCard = marioWM->GetTypeCard();
	}
	idWorld = 1;
	money = 0;
	points = 0;
	remainTime = COUNT_DOWN_TIME_DEFAULT/1000;
}
CHUD::~CHUD()
{

}

void CHUD::Render()
{
	CSprites* sprites = CSprites::GetInstance();
	RenderMainBoard(sprites);
	RenderPlayerIcon(sprites);
	RenderWorldId();
	RenderImminent(sprites);
	RenderPoints(sprites);
	RenderMoney(sprites);
	RenderTime(sprites);
	RenderLife();
	RenderCard();
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
	string strWorldId = to_string(idWorld);
	vector<LPSPRITE> spritesId = font->StringToSprites(strWorldId);
	for (int i = 1; i <= strWorldId.size(); i++)
	{
		spritesId.at(strWorldId.size() - i)->Draw(x + HUD_ID_X - i * FONT_WIDTH, y + HUD_ID_Y);
	}
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
void CHUD::RenderPoints(CSprites* sprites)
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
void CHUD::RenderMoney(CSprites* sprites)
{
	string strMoney = to_string(money);
	vector<LPSPRITE> spritesMoney = font->StringToSprites(strMoney);
	for (int i = 1; i <= strMoney.size(); i++)
	{
		spritesMoney.at(strMoney.size() - i)->Draw(x + HUD_MONEY_X - i * FONT_WIDTH, y + HUD_MONEY_Y);
	}
}
void CHUD::RenderTime(CSprites* sprites)
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
void CHUD::RenderLife()
{
	string strLife = to_string(life);
	vector<LPSPRITE> spritesLife = font->StringToSprites(strLife);
	for (int i = 1; i <= strLife.size(); i++)
	{
		spritesLife.at(strLife.size() - i)->Draw(x + HUD_LIFE_X - i * FONT_WIDTH, y + HUD_LIFE_Y);
	}
}
void CHUD::RenderCard()
{
	CSprites* sprites = CSprites::GetInstance();
	for (int i = 0; i < 3; i++)
	{
		switch (typeCard[i])
		{
		case ITEM_TYPE_STAR:
			sprites->Get(HUD_SPRITE_STAR_CARD)->Draw(x + HUD_CARD_X + i * HUD_CARD_WIDTH, y + HUD_CARD_Y);
			break; 
		case ITEM_TYPE_MUSHROOM:
			sprites->Get(HUD_SPRITE_MUSHROOM_CARD)->Draw(x + HUD_CARD_X + i * HUD_CARD_WIDTH, y + HUD_CARD_Y);
			break;
		case ITEM_TYPE_FLOWER:
			sprites->Get(HUD_SPRITE_FLOWER_CARD)->Draw(x + HUD_CARD_X + i * HUD_CARD_WIDTH, y + HUD_CARD_Y);
			break;
		}
	}
}
void CHUD::Update(DWORD dt)
{
	CScene* s = CGame::GetInstance()->GetCurrentScene();
	if (typeScene == HUD_TYPE_PLAYSCENE)
	{
		Imminent = mario->GetImminentStack();
		money = mario->GetMoney();
		points = mario->GetPoints();
		life = mario->GetLife();
		remainTime = ((CPlayScene*)s)->GetRemainTime() / 1000;
		if (remainTime < 0)
			remainTime = 0;
		typeCard = mario->GetTypeCard();
	}
	else
	{
		Imminent = 0;
		money = marioWM->GetMoney();
		points = marioWM->GetPoints();
		life = marioWM->GetLife();
		remainTime = 0;
		typeCard = marioWM->GetTypeCard();
	}
	//update x,y
	float cx, cy;
	CGame::GetInstance()->GetCamPos(cx, cy);
	x = cx;
	y = cy + CGame::GetInstance()->GetScreenHeight() - HUD_HEIGHT;
}

void CHUD::Reset()
{

}

