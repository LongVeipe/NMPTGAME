#pragma once
#include "Sprites.h"
#include <map>
using namespace std;

#define SCORE_SPRITE_100	999201
#define SCORE_SPRITE_200	999202
#define SCORE_SPRITE_400	999203
#define SCORE_SPRITE_800	999204
#define SCORE_SPRITE_1000	999205
#define SCORE_SPRITE_2000	999206
#define SCORE_SPRITE_4000	999207
#define SCORE_SPRITE_8000	999208
#define SCORE_SPRITE_1VP	999209

class CScores
{
private:
	map<int, LPSPRITE> scoresMap;

public:
	CScores();
	~CScores();
	LPSPRITE mapping(unsigned int a);
};

