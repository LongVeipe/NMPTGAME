#pragma once
#include "Scence.h"
#include "IntroSceneBackground.h"
#include "Mario.h"
#include "GameObject.h"
#include "Goomba.h"
#include "Star.h"
#include "Reward_LevelUp.h"
#include "Koopa_Small.h"


class CIntroScene: public CScene
{
private:
	CIntroSceneBackground* background;
	CCurtain* curtain;
	vector<LPGAMEOBJECT> objects;
	CMario* mario;
	CMario* luigi;
	CGoomba* goomba;
	CKoopa_Small* greenTurtoise;
	CStar* star;
	CReward_LevelUp* mushroom;
	CReward_LevelUp* leaf;


	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);

	void HandleMario();
	void HandleLuigi();
	void HandleGoomba();
	void HandleStar();
	void HandleLeaf();
	void HandleMushroom();
	void HandleTurtoise();
public:
	CIntroScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	void CalculateEnable();
	CMario* GetMario(){return mario;}

	void ShowMario();
	void ShowLuigi();
	void ShowNameOfGame();
	void ShowStar();
	void ShowGoomba();
	void ShowKoopa();
	void ShowLeaf();
	void ShowMushroom();
	void ShowGreenTurtoise();
};

class CIntroScenceKeyHandler : public CScenceKeyHandler
{
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	CIntroScenceKeyHandler(CScene* s) :CScenceKeyHandler(s) {};
};