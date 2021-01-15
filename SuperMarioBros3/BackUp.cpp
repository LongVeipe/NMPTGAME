#include "BackUp.h"
#include "Scence.h"
#include "Game.h"
#include "PlayScence.h"
#include "WorldMap.h"

CBackUp* CBackUp::__instance = NULL;
CBackUp* CBackUp::GetInstance()
{
	if (__instance == NULL) __instance = new CBackUp();
	return __instance;
}

void CBackUp::BackUpMario(CMario* mario)
{
	life = mario->GetLife();
	points = mario->GetPoints();
	level = mario->GetLevel();
	money = mario->GetMoney();
	for (int i = 0; i < 3; i++)
		typeCard[i] = mario->GetTypeCard()[i];
}

void CBackUp::BackUpMarioWM(CMarioWM* mario)
{
	life = mario->GetLife();
	points = mario->GetPoints();
	level = mario->GetLevel();
	money = mario->GetMoney();
	for (int i = 0; i < 3; i++)
		typeCard[i] = mario->GetTypeCard()[i];
}

void CBackUp::LoadBackUpMario(CMario* mario)
{
	mario->SetLife(life);
	mario->SetPoints(points);
	mario->SetLevel(level);
	mario->SetMoney(money);
	for (int i = 0; i < 3; i++)
		mario->AddCard(typeCard[i]);
}
void CBackUp::LoadBackUpMarioWM(CMarioWM* mario)
{
	mario->SetLife(life);
	mario->SetPoints(points);
	mario->SetLevel(level);
	mario->SetMoney(money);
	for (int i = 0; i < 3; i++)
		mario->AddCard(typeCard[i]);
}