#include "Koopas.h"
#include "Brick.h"
#include "Mario.h"
#include "Game.h"
#include "PlayScence.h"

CKoopas::CKoopas(float _x, float _y, int _type):CEnemy(_x, _y,_type)
{
	isTouchingGround = true;

}

CKoopas::~CKoopas()
{

}