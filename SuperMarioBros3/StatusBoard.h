#pragma once
#include <d3dx9.h>
#include <d3d9.h>
#include "Animations.h"
using namespace std;

class CStatusBoard
{
private:
	DWORD dt;
	int money;
	DWORD time;
	int Imminent;
	int point;
	LPANIMATION_SET animation_set;
	
public:
	CStatusBoard();
	~CStatusBoard();
	void Update(DWORD _dt);
	void Draw(float x, float y, int alpha);
	void Render();
	void Reset();
};