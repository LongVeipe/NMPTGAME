#pragma once

#include "GameObject.h"


class CKoopas : public CGameObject
{
protected:
	int type;
	float start_x;
	float limit_x;
public:
	bool isTouchGround;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) = 0;
	virtual void Render() = 0;

	CKoopas(float _x, float _y, int _type, float _limit_x);
	virtual ~CKoopas();

	int GetType() { return this->type; }
	void SetType(int _type) { type = _type; }
};