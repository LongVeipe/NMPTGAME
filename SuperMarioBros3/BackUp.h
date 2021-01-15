#pragma once
#include "mario.h"
#include "MarioWM.h"
class CBackUp
{
	static CBackUp* __instance;
	unsigned int life = 4;
	unsigned int money = 0;
	unsigned int points = 0;
	int level = MARIO_LEVEL_SMALL;
	int typeCard[3];
public:
	static CBackUp* GetInstance();
	void BackUpMario(CMario* mario);
	void BackUpMarioWM(CMarioWM* marioWM);
	void LoadBackUpMario(CMario* mario);
	void LoadBackUpMarioWM(CMarioWM* marioWM);

};

