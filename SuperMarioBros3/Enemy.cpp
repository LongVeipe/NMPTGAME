#include "Enemy.h"

CEnemy::CEnemy(float _x, float _y, int _type): CGameObject()
{
	x = _x;
	y = _y;
	type = _type;
	start_x = _x;
	start_type = _type;
	start_y = _y;
}

CEnemy::~CEnemy()
{

}
