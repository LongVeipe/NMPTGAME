#include "MovingEdge.h"
#include "Game.h"
#include "PlayScence.h"

CMovingEdge::CMovingEdge(float _x, float _y, float _limit_x)
{
	x = _x;
	y = _y;
	limit_x = _limit_x;
	SetState(MOVING_EDGE_STATE_MOVING);
}

void CMovingEdge::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == MOVING_EDGE_STATE_STOPPING)
		return;

	CGameObject::Update(dt, coObjects);


	int screenWidth = CGame::GetInstance()->GetScreenWidth();
	if (x >= limit_x )
	{
		x = limit_x;
		this->SetState(MOVING_EDGE_STATE_STOPPING);
		return;
	}
	else
	{
		CPlayScene* scene = (CPlayScene*)(CGame::GetInstance())->GetCurrentScene();
		CMario* mario = scene->GetPlayer();
		if (mario->x > x + screenWidth)
			this->SetState(MOVING_EDGE_STATE_STOPPING);
	}

	x += dx;

}

void CMovingEdge:: GetBoundingBox(float& l, float& t, float& r, float& b)
{

}

void CMovingEdge::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case MOVING_EDGE_STATE_MOVING:
		vx = MOVING_EDGE_SPEED;
		vy = 0;
		break;
	case MOVING_EDGE_STATE_STOPPING:
		vx = 0;
		break;
	}
}
