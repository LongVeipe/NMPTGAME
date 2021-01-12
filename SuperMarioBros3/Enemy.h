#pragma once
#include "GameObject.h"


class CEnemy : public CGameObject
{
protected:
	int type;
	int start_type;
	float start_x;
	float start_y;
	bool isReadyToEnable;
public:
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) = 0;
	virtual void Render() = 0;

	CEnemy(float _x, float _y, int _type);
	virtual ~CEnemy();

	int GetType() { return this->type; }
	void SetType(int _type) { type = _type; }
	virtual void Reset() = 0;
};

