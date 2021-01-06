#include "Scores.h"

CScores::CScores()
{
	CSprites* sprites = CSprites::GetInstance();
	scoresMap.insert(make_pair(100, sprites->Get(SCORE_SPRITE_100)));
	scoresMap.insert(make_pair(200, sprites->Get(SCORE_SPRITE_200)));
	scoresMap.insert(make_pair(400, sprites->Get(SCORE_SPRITE_400)));
	scoresMap.insert(make_pair(800, sprites->Get(SCORE_SPRITE_800)));
	scoresMap.insert(make_pair(1000, sprites->Get(SCORE_SPRITE_1000)));
	scoresMap.insert(make_pair(2000, sprites->Get(SCORE_SPRITE_2000)));
	scoresMap.insert(make_pair(4000, sprites->Get(SCORE_SPRITE_4000)));
	scoresMap.insert(make_pair(8000, sprites->Get(SCORE_SPRITE_8000)));
	scoresMap.insert(make_pair(8000, sprites->Get(SCORE_SPRITE_1VP)));
}
CScores::~CScores()
{
	
}
