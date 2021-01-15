#include "WorldMap.h"
#include <iostream>
#include <fstream>

#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Zone.h"
#include "Game.h"
#include "MarioWM.h"
#include "Station.h"
#include "Bush.h"
#include "BackUp.h"

using namespace std;

CWorldMap::CWorldMap(int id, LPCWSTR filePath) :CScene(id, filePath)
{
	key_handler = new CWorldMapKeyHandler(this);
	map = nullptr;
	player = nullptr;
	hud = nullptr;
}

/*
	Load scene resources from scene file (textures, sprites, animations and objects)
	See scene1.txt, scene2.txt for detail format specification
*/

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_TEXTURES 2
#define SCENE_SECTION_SPRITES 3
#define SCENE_SECTION_ANIMATIONS 4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS	6
#define SCENE_SECTION_MAP		7
#define SCENE_SECTION_ZONE		8

#define OBJECT_TYPE_MARIO		0
#define OBJECT_TYPE_STATION		1
#define OBJECT_TYPE_BUSH		2

#define MAX_SCENE_LINE 1024


void CWorldMap::_ParseSection_TEXTURES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; // skip invalid lines

	int texID = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);
	int R = atoi(tokens[2].c_str());
	int G = atoi(tokens[3].c_str());
	int B = atoi(tokens[4].c_str());

	CTextures::GetInstance()->Add(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));
}
void CWorldMap::_ParseSection_MAP(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 9) return; // skip invalid lines

	int idMap = atoi(tokens[0].c_str());
	int tileWidth = atoi(tokens[1].c_str());
	int tileHeight = atoi(tokens[2].c_str());
	int tRTileSet = atoi(tokens[3].c_str());
	int tCTileSet = atoi(tokens[4].c_str());
	int tRMap = atoi(tokens[5].c_str());
	int tCMap = atoi(tokens[6].c_str());
	int totalTiles = atoi(tokens[7].c_str());
	wstring MatrixPath = ToWSTR(tokens[8]);

	this->map = new Map(idMap, tileWidth, tileHeight, tRTileSet, tCTileSet, tRMap, tCMap, totalTiles);
	map->LoadMatrix(MatrixPath.c_str());
	map->CreateTilesFromTileSet();
}
void CWorldMap::_ParseSection_ZONE(string line)
{
	vector<string> tokens = split(line);
	if (tokens.size() < 5) return;
	int id = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());

	CZone* zone = new CZone(l, t, r, b);
	CZones::GetInstance()->Add(id, zone);
}
void CWorldMap::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPDIRECT3DTEXTURE9 tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return;
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}
void CWorldMap::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}
void CWorldMap::_ParseSection_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

	int ani_set_id = atoi(tokens[0].c_str());

	LPANIMATION_SET s = new CAnimationSet();

	CAnimations* animations = CAnimations::GetInstance();

	for (int i = 1; i < tokens.size(); i++)
	{
		int ani_id = atoi(tokens[i].c_str());

		LPANIMATION ani = animations->Get(ani_id);
		s->push_back(ani);
	}

	CAnimationSets::GetInstance()->Add(ani_set_id, s);
}
void CWorldMap::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());

	int ani_set_id = atoi(tokens[3].c_str());

	CAnimationSets* animation_sets = CAnimationSets::GetInstance();

	CGameObject* obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_MARIO:
		if (player != NULL)
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		obj = new CMarioWM(x, y);
		player = (CMarioWM*)obj;

		DebugOut(L"[INFO] Player object created!\n");
		break;
	case OBJECT_TYPE_STATION:
	{
		int l = atoi(tokens[4].c_str());
		int t = atoi(tokens[5].c_str());
		int r = atoi(tokens[6].c_str());
		int b = atoi(tokens[7].c_str());
		int id = atoi(tokens[8].c_str());
		obj = new CStation(x, y, l, t, r, b, id);
		break;
	}
	case OBJECT_TYPE_BUSH:
		obj = new CBush();
		break;
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	obj->SetPosition(x, y);

	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

	obj->SetAnimationSet(ani_set);
	objects.push_back(obj);
}

void CWorldMap::Load()
{
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[TEXTURES]") { section = SCENE_SECTION_TEXTURES; continue; }
		if (line == "[MAP]") { section = SCENE_SECTION_MAP; continue; }
		if (line == "[ZONE]") { section = SCENE_SECTION_ZONE; continue; }
		if (line == "[SPRITES]") { section = SCENE_SECTION_SPRITES; continue; }
		if (line == "[ANIMATIONS]") { section = SCENE_SECTION_ANIMATIONS; continue; }
		if (line == "[ANIMATION_SETS]") { section = SCENE_SECTION_ANIMATION_SETS; continue; }
		if (line == "[OBJECTS]") { section = SCENE_SECTION_OBJECTS; continue; }
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
		case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		case SCENE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
		case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		case SCENE_SECTION_MAP: _ParseSection_MAP(line); break;
		case SCENE_SECTION_ZONE: _ParseSection_ZONE(line); break;
		}
	}

	f.close();

	CBackUp::GetInstance()->LoadBackUpMarioWM(player);

	CTextures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);

	hud = new CHUD(HUD_TYPE_WORLDMAP);
}

void CWorldMap::Update(DWORD dt)
{
	 //We know that Mario is the first object in the list hence we won't add him into the colliable object list

	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 0; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}
	for (size_t i = 0; i < objects.size(); i++)
	{

		objects[i]->Update(dt, &coObjects);
	}

	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return;

	SetCamera();
	//update HUD
	if (hud != nullptr)
		hud->Update(dt);
}
void CWorldMap::SetCamera()
{

	float cx, cy;
	player->GetPosition(cx, cy);
	CGame* game = CGame::GetInstance();
	CZone* zone = CZones::GetInstance()->Get(idZone);
	int lZone, tZone, rZone, bZone;
	zone->GetZone(lZone, tZone, rZone, bZone);


	CGame::GetInstance()->SetCamPos(round(lZone), round(tZone));
}

void CWorldMap::Render()
{
	if (map)
		this->map->Render();
	for (int i = 0; i < objects.size(); i++)
	{
		objects[i]->Render();
	}

	if(hud)
		hud->Render();
}

/*
	Unload current scene
*/
void CWorldMap::Unload()
{
	if(player)
		CBackUp::GetInstance()->BackUpMarioWM(player);
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	player = NULL;

	delete map;
	map = nullptr;

	delete hud;
	hud = nullptr;

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}




void CWorldMapKeyHandler::OnKeyDown(int KeyCode)
{
	DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	CMarioWM* mario = ((CWorldMap*)scence)->GetPlayer();
	switch (KeyCode)
	{
	case DIK_UP:
		mario->SetState(MARIOWM_STATE_WALKING_UP);
		break;
	case DIK_DOWN:
		mario->SetState(MARIOWM_STATE_WALKING_DOWN);
		break;
	case DIK_LEFT:
		mario->SetState(MARIOWM_STATE_WALKING_LEFT);
		break;
	case DIK_RIGHT:
		mario->SetState(MARIOWM_STATE_WALKING_RIGHT);
		break;
	case DIK_S:
		CGame::GetInstance()->SwitchScene(mario->targetScene);
		break;
	}
}

void CWorldMapKeyHandler::OnKeyUp(int KeyCode)
{
}
void CWorldMapKeyHandler::KeyState(BYTE* states)
{
	CGame* game = CGame::GetInstance();
	CMarioWM* mario = ((CWorldMap*)scence)->GetPlayer();

	// disable control key when Mario die 

	if (game->IsKeyDown(DIK_RIGHT))
	{
		
	}
	else if (game->IsKeyDown(DIK_LEFT))
	{
		
	}
	else if (game->IsKeyDown(DIK_SPACE))
	{
		
	}
}