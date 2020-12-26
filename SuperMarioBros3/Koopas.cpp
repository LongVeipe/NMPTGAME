#include "Koopas.h"
#include "Brick.h"
#include "Mario.h"
#include "Game.h"
#include "PlayScence.h"

CKoopas::CKoopas(float _x, float _y, int _type, float _limit_x):CGameObject()
{
	x = _x;
	y = _y;
	type = _type;
	limit_x = _limit_x;
}

CKoopas::~CKoopas()
{

}