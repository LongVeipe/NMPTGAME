#pragma once
#include "GameObject.h"


class CEnemy : public CGameObject
{
protected:
	float start_x;
	float final_x;
	int level;
	int type;
public:
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) = 0;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render() = 0;
	CEnemy(float start_x, float final_x,  int type);
	virtual void SetState(int state);
	virtual ~CEnemy();
	virtual int GetType() { return this->type; }
	virtual void SetType(int _type) { this->type = _type; }
	virtual int GetLevel() { return this->level; }
	virtual void SetLevel(int _level) { this->level = _level; }
};