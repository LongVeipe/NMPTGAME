#pragma once
#include "MarioWM.h"
#include "Scence.h"
#include "Map.h"
#include "HUD.h"
#include "Portal.h"
class CWorldMap: public CScene
{
private:
	CMarioWM* player;
	vector<LPGAMEOBJECT> objects;
	Map* map;
	CHUD* hud;
	int idZone = 1;


	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_MAP(string line);
	void _ParseSection_ZONE(string line);

	void SetCamera();
public:

	CWorldMap(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	void TransferZone(CPortal* portal);

	CMarioWM* GetPlayer() { return player; }
	Map* GetMap() { return map; }
	CHUD* GetHUD() { return hud; }
	vector<LPGAMEOBJECT> GetObjects() { return objects; }
};

class CWorldMapKeyHandler : public CScenceKeyHandler
{
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	CWorldMapKeyHandler(CScene* s) :CScenceKeyHandler(s) {};
};
