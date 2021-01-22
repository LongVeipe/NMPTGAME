#include <iostream>
#include <fstream>

#include "PlayScence.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Zone.h"
#include "Portal.h"
#include "Coin.h"
#include "Bullet.h"
#include "Reward_LevelUp.h"
#include "Plant_Fire.h"
#include "Plant_Normal.h"
#include "Koopa_Small.h"
#include "RewardBox.h"
#include "PointsEffect.h"
#include "BackUp.h"
#include "Item.h"

using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath, int idWM):CScene(id, filePath)
{
	idWorldMap = idWM;
	key_handler = new CPlayScenceKeyHandler(this);
	map = nullptr;
	player = nullptr;
	hud = nullptr;
	remainTime = COUNT_DOWN_TIME_DEFAULT;
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
#define SCENE_SECTION_GRID		9

#define OBJECT_TYPE_MARIO			0
#define OBJECT_TYPE_BRICK			1
#define OBJECT_TYPE_REWARD_BOX	2
#define OBJECT_TYPE_GOOMBA			3
#define OBJECT_TYPE_KOOPA_SMALL		4
#define OBJECT_TYPE_COIN			5
#define OBJECT_TYPE_PLANT_FIRE		6
#define OBJECT_TYPE_PLANT_NORMAL	7
#define OBJECT_TYPE_ITEM			8

#define OBJECT_TYPE_PORTAL	50

#define MAX_SCENE_LINE 1024


void CPlayScene::_ParseSection_TEXTURES(string line)
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
void CPlayScene::_ParseSection_MAP(string line)
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
void CPlayScene::_ParseSection_ZONE(string line)
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
void CPlayScene::_ParseSection_SPRITES(string line)
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
void CPlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (unsigned int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i+1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}
void CPlayScene::_ParseSection_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

	int ani_set_id = atoi(tokens[0].c_str());

	LPANIMATION_SET s = new CAnimationSet();

	CAnimations *animations = CAnimations::GetInstance();

	for (unsigned int i = 1; i < tokens.size(); i++)
	{
		int ani_id = atoi(tokens[i].c_str());
		
		LPANIMATION ani = animations->Get(ani_id);
		s->push_back(ani);
	}

	CAnimationSets::GetInstance()->Add(ani_set_id, s);
}
void CPlayScene::_ParseSection_GRID(string line)
{
	ifstream gridFile;
	gridFile.open(line);

	int gridCols = -1;
	int gridRows = -1;

	if (!gridFile)
	{
		DebugOut(L"Failed to open grid file\n");
		return;
	}

	gridFile >> gridRows >> gridCols;


	if (gridCols == -1 || gridRows == -1)
		return;

	grid = new CGrid(gridRows, gridCols);


	char str[MAX_SCENE_LINE];
	while (gridFile.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#')
			continue;

		_ParseObjectsFromGrid(line);
	}

	gridFile.close();


	DebugOut(L"\nParseSection_GRID: Done\n");
}
void CPlayScene::_ParseObjectsFromGrid(string line)
{
	vector<string> tokens = split(line);


	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	float x =(float) atof(tokens[1].c_str());
	float y = (float)atof(tokens[2].c_str());

	int ani_set_id = atoi(tokens[3].c_str());

	CAnimationSets * animation_sets = CAnimationSets::GetInstance();

	CGameObject *obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_MARIO:
		if (player!=NULL) 
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		obj = new CMario(x,y); 
		player = (CMario*)obj;  

		DebugOut(L"[INFO] Player object created!\n");
		break;
	//case OBJECT_TYPE_GOOMBA: obj = new CGoomba(); break;
	case OBJECT_TYPE_BRICK:
	{
		int type = atoi(tokens[4].c_str());
		obj = new CBrick(x, y, type);
		break;
	}
	case OBJECT_TYPE_REWARD_BOX:
	{
		int type = atoi(tokens[4].c_str());
		int rewardType = atoi(tokens[5].c_str());
		obj = new CRewardBox(x, y, type, rewardType);
		break;
	}
	case OBJECT_TYPE_KOOPA_SMALL:
	{
		int typeKoopa = atoi(tokens[4].c_str());
		obj = new CKoopa_Small(x, y, typeKoopa);
		listEnemies.push_back((CKoopa_Small*)obj);
		break;
	}
	case OBJECT_TYPE_GOOMBA:
	{
		int typeGoomba = atoi(tokens[4].c_str());
		obj = new CGoomba(x, y, typeGoomba);
		listEnemies.push_back((CGoomba*)obj);
		break;
	}
	case OBJECT_TYPE_PLANT_FIRE:
	{
		float limit_y =(float) atof(tokens[4].c_str());
		int type = atoi(tokens[5].c_str());
		obj = new CPlant_Fire(x, y, limit_y, type);
		break;
	}
	case OBJECT_TYPE_PLANT_NORMAL:
	{
		float limit_y = (float)atof(tokens[4].c_str());
		int type = atoi(tokens[5].c_str());
		obj = new CPlant_Normal(x, y, limit_y, type);
		break;
	}
	case OBJECT_TYPE_COIN: 
	{
		obj = new CCoin(x, y);
		break;
	}
	case OBJECT_TYPE_ITEM:
	{
		obj = new CItem();
		break;
	}
	case OBJECT_TYPE_PORTAL:
	{
		float r =(float) atof(tokens[4].c_str());
		float b = (float)atof(tokens[5].c_str());
		int targetZone = atoi(tokens[6].c_str());
		float targetX =(float) atof(tokens[7].c_str());
		float targetY =(float) atof(tokens[8].c_str());
		int type = atoi(tokens[9].c_str());
		obj = new CPortal(x, y, r, b, targetZone, targetX, targetY, type);
	}
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

	int gridCol = (int)atoi(tokens[tokens.size() - 1].c_str());
	int gridRow = (int)atoi(tokens[tokens.size() - 2].c_str());
	CUnit* unit = new CUnit(gridRow, gridCol, grid, obj);
}
void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	float x = (float)atof(tokens[1].c_str());
	float y = (float)atof(tokens[2].c_str());

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
		obj = new CMario(x, y);
		player = (CMario*)obj;

		DebugOut(L"[INFO] Player object created!\n");
		break;
		//case OBJECT_TYPE_GOOMBA: obj = new CGoomba(); break;
	case OBJECT_TYPE_BRICK:
	{
		int type = atoi(tokens[4].c_str());
		obj = new CBrick(x, y, type);
		break;
	}
	case OBJECT_TYPE_REWARD_BOX:
	{
		int type = atoi(tokens[4].c_str());
		int rewardType = atoi(tokens[5].c_str());
		obj = new CRewardBox(x, y, type, rewardType);
		break;
	}
	case OBJECT_TYPE_KOOPA_SMALL:
	{
		int typeKoopa = atoi(tokens[4].c_str());
		obj = new CKoopa_Small(x, y, typeKoopa);
		break;
	}
	case OBJECT_TYPE_GOOMBA:
	{
		int typeGoomba = atoi(tokens[4].c_str());
		obj = new CGoomba(x, y, typeGoomba);
		break;
	}
	case OBJECT_TYPE_PLANT_FIRE:
	{
		float limit_y = (float)atof(tokens[4].c_str());
		int type = atoi(tokens[5].c_str());
		obj = new CPlant_Fire(x, y, limit_y, type);
		break;
	}
	case OBJECT_TYPE_PLANT_NORMAL:
	{
		float limit_y = (float)atof(tokens[4].c_str());
		int type = atoi(tokens[5].c_str());
		obj = new CPlant_Normal(x, y, limit_y, type);
		break;
	}
	case OBJECT_TYPE_COIN:
	{
		obj = new CCoin(x, y);
		break;
	}
	case OBJECT_TYPE_ITEM:
	{
		obj = new CItem();
		break;
	}
	case OBJECT_TYPE_PORTAL:
	{
		float r = (float)atof(tokens[4].c_str());
		float b = (float)atof(tokens[5].c_str());
		int targetZone = atoi(tokens[6].c_str());
		float targetX = (float)atof(tokens[7].c_str());
		float targetY = (float)atof(tokens[8].c_str());
		int type = atoi(tokens[9].c_str());
		obj = new CPortal(x, y, r, b, targetZone, targetX, targetY, type);
	}
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
void CPlayScene::Load()
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
		if (line == "[OBJECTS]") { break; }
		if (line == "[GRID]") { section = SCENE_SECTION_GRID; continue; }
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
			//case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
			case SCENE_SECTION_MAP: _ParseSection_MAP(line); break;
			case SCENE_SECTION_ZONE: _ParseSection_ZONE(line); break;
			case SCENE_SECTION_GRID: _ParseSection_GRID(line); break;
		}
	}

	f.close();

	CTextures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));

	CBackUp::GetInstance()->LoadBackUpMario(player);

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);

	hud = new CHUD(HUD_TYPE_PLAYSCENE);
	SetCamera();
}

void CPlayScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 
	GetListUnitFromGrid();
	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 0; i < listUnits.size(); i++)
		coObjects.push_back(listUnits.at(i)->GetObj());

	for (size_t i = 0; i < listUnits.size(); i++)
	{
		LPGAMEOBJECT 
			object = listUnits[i]->GetObj();
		if (!dynamic_cast<CBrick*>(object))
		{
			object->Update(dt, &coObjects);

				float newx, newy;
			listUnits[i]->GetObj()->GetPosition(newx, newy);
			listUnits[i]->Move(newx, newy);
		}
	}

	for (size_t i = 0; i < listEnemies.size(); i++)
		if (listEnemies[i]->IsInCamera() == false)
			listEnemies[i]->Update(dt, &coObjects);

	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return;


	// Update camera to follow mario
	SetCamera();

	//update remain time
	remainTime -= dt;
	if (remainTime < 0)
		remainTime = 0;

	//update HUD
	hud->Update(dt);

	//update pointsEffects
	CPointsEffects::GetInstance()->Update(dt);

	//out to WorldMap when mario die
	if ( player->y > CZones::GetInstance()->Get(idZone)->GetBottom())
		CGame::GetInstance()->SwitchScene(idWorldMap);
}

void CPlayScene::SetCamera()
{

	float cx, cy;
	player->GetPosition(cx, cy);
	CGame* game = CGame::GetInstance();
	CZone* zone = CZones::GetInstance()->Get(idZone);
	//cx
	int lZone, tZone, rZone, bZone;
	zone->GetZone(lZone, tZone, rZone, bZone);
	int sWidth = game->GetScreenWidth();
	if (cx >rZone  - sWidth / 2)
		cx = float(rZone - sWidth);
	else if (cx < lZone + sWidth / 2)
		cx = (float)lZone;
	else
		cx -= sWidth / 2;
	//cy
	int sHeight = game->GetScreenHeight();
	if (cy < tZone + sHeight / 2)
		cy = (float)tZone;
	else if (cy > bZone - sHeight / 2)
		cy =(float) (bZone - sHeight);
	else
		cy -= sHeight / 2;
	if (!player->IsSwingTail)
		CGame::GetInstance()->SetCamPos(round(cx), round(cy));
}

void CPlayScene::Render()
{
	if (map)
		this->map->Render();

	for (int i = listUnits.size()-1; i >= 0; i--)
	{
		listUnits[i]->GetObj()->Render();
	}

	hud->Render();
	CPointsEffects::GetInstance()->Render();
	if (noti)
		noti->Render();
}

/*
	Unload current scene
*/
void CPlayScene::Unload()
{
	if(player)
		CBackUp::GetInstance()->BackUpMario(player);
	for (unsigned int i = 0; i < objects.size(); i++)
		delete objects[i];
	
	objects.clear();
	player = NULL;

	delete map;
	map = nullptr;

	delete hud;
	hud = nullptr;

	delete noti;
	noti = nullptr;

	delete grid;
	grid = nullptr;

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}
void CPlayScene::TransferZone(CPortal* portal)
{
	int tartgetZone = portal->GetTartgetZone();
	float targetX = portal->GetTargetX();
	float targetY = portal->GetTargetY();

	player->SetPosition(targetX, targetY);
	idZone = tartgetZone;
}
void CPlayScene::GetListUnitFromGrid()
{
	listUnits.clear();
	float cx = 0, cy = 0;
	CGame::GetInstance()->GetCamPos(cx, cy);
	grid->Get(cx, cy, listUnits);
}

void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	CMario *mario = ((CPlayScene*)scence)->GetPlayer();
	switch (KeyCode)
	{
	case DIK_S:
		if (mario->GetLevel() == MARIO_LEVEL_RACCOON)
		{
			if (mario->IsRaccoonReadyFly())
			{
				mario->RaccoonStartFlyHigh();
			}
			else if (mario->IsRaccoonCanFlyHigh)
			{
				mario->SetState(MARIO_STATE_JUMP);
			}
			else if (mario->IsFalling && !mario->IsRaccoonCanFlyHigh)
			{
 				if (!mario->IsFlying)
					mario->SlowFall();
			}


		}
		if (mario->IsTouchingGround == true)
		{
			mario->SetJumpStack(0);
			mario->IsReadyJump = true;
			mario->SetState(MARIO_STATE_JUMP);
			mario->UpJumpStack();
			mario->IsTouchingGround = false;
			
		}
		break;
	case DIK_Q: 
		mario->Reset();
		break;
	case DIK_A:
		if (mario->GetLevel() == MARIO_LEVEL_FIRE)
			mario->StartThrowFire();
		else if (mario->GetLevel() == MARIO_LEVEL_RACCOON)
			mario->StartSwingTail();
		break;
	case DIK_DOWN:
		mario->SetState(MARIO_STATE_BEND_DOWN);
		break;
	case DIK_Z:
		int newLevel = mario->GetLevel() + 1;
		if (newLevel > MARIO_MAX_LEVEL)
			newLevel = 1;
		float cur_x, cur_y;
		mario->GetPosition(cur_x, cur_y);
		mario->SetPosition(cur_x, cur_y - 16);
		mario->SetLevel(newLevel);
		break;
	}
}
void CPlayScenceKeyHandler::OnKeyUp(int KeyCode)
{

	CMario* mario = ((CPlayScene*)scence)->GetPlayer();
	switch (KeyCode)
	{
	case DIK_DOWN:
		mario->StandUp();
		break;
	case DIK_S:
		mario->SetJumpStack(MARIO_MAX_JUMPIMG_STACKS);
		mario->IsReadyJump = false;
		break;
	case DIK_RIGHT:
		mario->SetState(MARIO_STATE_IDLE);
		break;
	case DIK_LEFT:
		mario->SetState(MARIO_STATE_IDLE);
		break;
	case DIK_A:
		mario->IsReadyHolding = false;
		if (mario->IsHolding)
			mario->StartKick();
		mario->IsHolding = false;
		break;
	}
}

void CPlayScenceKeyHandler::KeyState(BYTE *states)
{
	CGame *game = CGame::GetInstance();
	CMario *mario = ((CPlayScene*)scence)->GetPlayer();

	// disable control key when Mario die 
	
	if (mario->GetState() == MARIO_STATE_DIE) return;
	
	if (game->IsKeyDown(DIK_RIGHT))
	{
		mario->SetState(MARIO_STATE_WALKING_RIGHT);

		if (game->IsKeyDown(DIK_S))
		{
			if (mario->IsReadyJump == true && mario->GetJumpStack() < MARIO_MAX_JUMPIMG_STACKS)
			{

				mario->SetState(MARIO_STATE_JUMP);
				mario->UpJumpStack();
			}
			mario->downImminent();
		}
		else if (game->IsKeyDown(DIK_A))
		{
			mario->upImminent();
		}
		else
			mario->downImminent();
	}
	else if (game->IsKeyDown(DIK_LEFT))
	{
		mario->SetState(MARIO_STATE_WALKING_LEFT);
		if (game->IsKeyDown(DIK_S))
		{
			if (mario->IsReadyJump == true && mario->GetJumpStack() < MARIO_MAX_JUMPIMG_STACKS)
			{

				mario->SetState(MARIO_STATE_JUMP);
				mario->UpJumpStack();
			}
			mario->downImminent();
		}
		else if (game->IsKeyDown(DIK_A))
		{
			mario->upImminent();
		}
		else
		{
			mario->downImminent();
		}
	}
	else if(game->IsKeyDown(DIK_S))
	{ 
		if (mario->IsReadyJump == true && mario->GetJumpStack() < MARIO_MAX_JUMPIMG_STACKS)
		{
			mario->SetState(MARIO_STATE_JUMP);
			mario->UpJumpStack();
			if(game->IsKeyDown(DIK_RIGHT))
				mario->SetState(MARIO_STATE_WALKING_RIGHT);
			else if(game->IsKeyDown(DIK_LEFT))
				mario->SetState(MARIO_STATE_WALKING_LEFT);
			mario->downImminent();
		}
	}
	else 
	{ 
		if(mario->IsTouchingGround)
			mario->SetState(MARIO_STATE_IDLE);
		mario->downImminent();
	}
	if (game->IsKeyDown(DIK_A))
	{
		mario->IsReadyHolding = true;
	}
	if (game->IsKeyDown(DIK_DOWN))
	{
		if(mario->GetLevel() == MARIO_LEVEL_BIG || mario->GetLevel() == MARIO_LEVEL_RACCOON)
			mario->IsReadyDucking = true;
	}
}