#pragma once
#include "Animations.h"
#include "Mario.h"
#include "Font.h"
//using namespace std;

#define HUD_SPRITE_MAIN_BOARD					999001
#define HUD_SPRITE_EMPTY_CARD					999003
#define HUD_SPRITE_MUSHROOM_CARD				999004
#define HUD_SPRITE_FLOWER_CARD					999005
#define HUD_SPRITE_STAR_CARD					999006
#define HUD_SPRITE_MARIO_ICON					999007
#define HUD_SPRITE_LUIGI_ICON					999008
#define HUD_SPRITE_ACTIVE_NORMAL_IMMINENT		999009
#define HUD_SPRITE_ACTIVE_LAST_IMMINENT			999010
#define HUD_SPRITE_INACTIVE_NORMAL_IMMINENT		999011
#define HUD_SPRITE_INACTIVE_LAST_IMMINENT		999012


#define HUD_PLAYER_ICON_X		9
#define HUD_PLAYER_ICON_Y		15
#define HUD_IMMINENT_X			57
#define HUD_IMMINENT_Y			7
#define HUD_WORLD_ID_X			42
#define HUD_WORLD_ID_Y			7
#define HUD_POINTS_X				57
#define HUD_POINTS_Y				15
#define	HUD_MONEY_X				153
#define HUD_MONEY_Y				7
#define HUD_TIME_X				129
#define HUD_TIME_Y				15


#define HUD_MAX_POINTS_NUMBER_OF_DIGIT		7
#define HUD_MAX_TIME_NUMBER_OF_DIGIT		3

#define HUD_HEIGHT	30
#define HUD_WIDTH	234
#define HUD_NORMAL_IMMINENT_WIDTH	8


class CHUD
{
private:
	float x;
	float y;
	CMario* player;
	int idWorld;
	int typePlayer;
	unsigned int money;
	DWORD remainTime;
	int Imminent;
	unsigned int points;
	LPANIMATION_SET animation_set;
	
public:
	CHUD();
	~CHUD();
	void Update(DWORD _dt);
	void Render();
	void RenderMainBoard(CSprites* sprites);
	void RenderPlayerIcon(CSprites* sprites);
	void RenderImminent(CSprites* sprites);
	void RenderPoints(CSprites* sprites, CFont* font);
	void RenderWorldId();
	void RenderTime(CSprites* sprites, CFont* font);
	void RenderMoney(CSprites* sprites, CFont* font);
	void Reset();
};