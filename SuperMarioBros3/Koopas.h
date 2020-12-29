#pragma once

#include "Enemy.h"


class CKoopas : public CEnemy
{
public:
	bool isTouchingGround;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) = 0;
	virtual void Render() = 0;

	CKoopas(float _x, float _y, int _type);
	virtual ~CKoopas();
	virtual void Reset() = 0;
};