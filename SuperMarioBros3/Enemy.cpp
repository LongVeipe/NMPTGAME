#include "Enemy.h"
#include "Brick.h"
#include "Mario.h"
CEnemy::CEnemy(float _start_x,float _final_x, int _type):CGameObject()
{

	start_x = _start_x;
	final_x = _final_x;
	type = _type;
}



void CEnemy::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
}


void CEnemy::SetState(int state)
{
	CGameObject::SetState(state);
	
}

CEnemy::~CEnemy()
{

}

