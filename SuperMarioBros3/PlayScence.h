#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scence.h"
#include "GameObject.h"
#include "Brick.h"
#include "Mario.h"
#include "Goomba.h"
#include "Koopas.h"
#include "Map.h"
#include "HUD.h"
#include "Font.h"
#include "Portal.h"
#include "Item.h"
#include "EndSceneNotification.h"

#define COUNT_DOWN_TIME_DEFAULT			300000

class CPlayScene : public CScene
{
protected:
	CMario* player;					// A play scene has to have player, right? 

	vector<LPGAMEOBJECT> objects;
	Map* map;
	CHUD* hud;
	DWORD remainTime;
	int idZone = 1;
	int idWorldMap = 1;
	CEndSceneNotification* noti = nullptr;


	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_MAP(string line);
	void _ParseSection_ZONE(string line);

	void SetCamera();
public:

	CPlayScene(int id, LPCWSTR filePath, int _idWorldMap);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	CMario* GetPlayer() { return player; }
	Map* GetMap() { return map; }
	CHUD* GetHUD() { return hud; }
	DWORD GetRemainTime() { return remainTime; }
	vector<LPGAMEOBJECT> GetObjects() { return objects; }
	void SetNoti(CEndSceneNotification* n) { noti = n; }
	void TransferZone(CPortal* portal);
	
	//friend class CPlayScenceKeyHandler;
};

class CPlayScenceKeyHandler : public CScenceKeyHandler
{
public: 
	virtual void KeyState(BYTE *states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	CPlayScenceKeyHandler(CScene *s) :CScenceKeyHandler(s) {};
};

