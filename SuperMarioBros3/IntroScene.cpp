#include <iostream>
#include <fstream>

#include "IntroScene.h"
#include "Textures.h"
#include "Utils.h"
#include <dinput.h>
#include "Brick.h"
#include "Game.h"

using namespace std;

#define INTROSCENE_SECTION_UNKNOWN			-1
#define INTROSCENE_SECTION_TEXTURES			1
#define INTROSCENE_SECTION_SPRITES			2
#define INTROSCENE_SECTION_ANIMATIONS		3
#define INTROSCENE_SECTION_ANIMATION_SETS	4
#define INTROSCENE_SECTION_OBJECTS			5

#define OBJECT_TYPE_MARIO	0
#define OBJECT_TYPE_LUIGI	1
#define OBJECT_TYPE_BRICK	2
#define OBJECT_TYPE_GOOMBA	3
#define OBJECT_TYPE_KOOPA	4
#define OBJECT_TYPE_STAR	5
#define OBJECT_TYPE_LEAF	6
#define OBJECT_TYPE_MUSHROOM	7

#define MAX_SCENE_LINE 1024

#define STAGE_PULL_UP_CURTAIN		1
#define STAGE_MARIO_WALK_IN_MIDDLE	2
#define STAGE_PULL_DOWN_NAMEOFGAME	3

#define BIGBUSH_SPRITE_ID	20010

CIntroScene::CIntroScene(int id, LPCWSTR filePath): CScene(id, filePath)
{
	key_handler = new CIntroScenceKeyHandler(this);
}

void CIntroScene::_ParseSection_TEXTURES(string line)
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
void CIntroScene::_ParseSection_SPRITES(string line)
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
void CIntroScene::_ParseSection_ANIMATIONS(string line)
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
void CIntroScene::_ParseSection_ANIMATION_SETS(string line)
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
void CIntroScene::_ParseSection_OBJECTS(string line)
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
		if (mario != NULL)
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		obj = new CMario(x, y);
		mario = (CMario*)obj;
		mario->SetType(MARIO);
		mario->SetLevel(MARIO_LEVEL_BIG);
		mario->state = MARIO_STATE_WALKING_LEFT;
		mario->nx = -1;
		mario->vx = 0;
		DebugOut(L"[INFO] Player object created!\n");
		break;
	case OBJECT_TYPE_LUIGI:
		if (luigi != NULL)
		{
			DebugOut(L"[ERROR] LUIGI object was created before!\n");
			return;
		}
		obj = new CMario(x, y);
		luigi = (CMario*)obj;
		luigi->SetType(LUIGI);
		luigi->SetLevel(MARIO_LEVEL_BIG);
		luigi->state = MARIO_STATE_WALKING_RIGHT;
		luigi->nx = 1;
		luigi->vx = 0;
		DebugOut(L"[INFO] Player object created!\n");
		break;
	case OBJECT_TYPE_BRICK:
	{
		int _type = atoi(tokens[4].c_str());
		obj = new CBrick(x, y, _type);
		break;
	}
	case OBJECT_TYPE_STAR:
		obj = new CStar();
		star = (CStar*)obj;
		break;
	case OBJECT_TYPE_GOOMBA:
	{
		float typeGoomba = atof(tokens[4].c_str());
		obj = new CGoomba(x, y, typeGoomba);
		goomba = (CGoomba*)obj;
		goomba->SetState(GOOMBA_STATE_IDLE);
		break;
	}
	case OBJECT_TYPE_KOOPA:
	{
		float typeKoopa = atof(tokens[4].c_str());
		obj = new CKoopa_Small(x, y, typeKoopa);
		greenTurtoise = (CKoopa_Small*)obj;
		break;
	}
	case OBJECT_TYPE_LEAF:
	{
		obj = new CReward_LevelUp(x, y);
		leaf = (CReward_LevelUp*)obj;
		leaf->SetType(REWARD_LEVEL_UP_TYPE_SUPER_LEAF);
		break;
	}
	case OBJECT_TYPE_MUSHROOM:
	{
		obj = new CReward_LevelUp(x, y);
		mushroom = (CReward_LevelUp*)obj;
		mushroom->SetType(REWARD_LEVEL_UP_TYPE_SUPER_MUSHROOM);
		break;
	}
	}
	obj->SetPosition(x, y);

	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

	obj->SetAnimationSet(ani_set);
	obj->IsEnable = false;
	objects.push_back(obj);
}

void CIntroScene::Load()
{

	background = new CIntroSceneBackground();
	background->IsEnable = true;
	objects.push_back(background);


	menu = new CMenuIntro();
	menu->IsEnable = false;
	objects.push_back(menu);
	menu->SetPosition(72, 145);

	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = INTROSCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[TEXTURES]") { section = INTROSCENE_SECTION_TEXTURES; continue; }
		if (line == "[SPRITES]") { section = INTROSCENE_SECTION_SPRITES; continue; }
		if (line == "[ANIMATIONS]") { section = INTROSCENE_SECTION_ANIMATIONS; continue; }
		if (line == "[ANIMATION_SETS]") { section = INTROSCENE_SECTION_ANIMATION_SETS; continue; }
		if (line == "[OBJECTS]") { section = INTROSCENE_SECTION_OBJECTS; continue; }
		if (line[0] == '[') { section = INTROSCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case INTROSCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
		case INTROSCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case INTROSCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		case INTROSCENE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
		case INTROSCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		}
	}

	f.close();

	CTextures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);

	background->InitBackground();
	curtain = new CCurtain();
	objects.push_back(curtain);
}
void CIntroScene::Render()
{
	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Render();
	}
	if (bigBush != nullptr)
		bigBush->Draw(193, 90);
}
void CIntroScene::Update(DWORD dt)
{
	HandleMario();
	HandleLuigi();
	HandleStar();
	HandleLeaf();
	HandleGoomba();
	HandleMushroom();
	HandleTurtoise();
	
	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 1; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}
	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
	}
	//DebugOut(L"isFlying: %d \n", mario->IsFlying);
}
void CIntroScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();

	mario = NULL;
	luigi = nullptr;
	background = nullptr;
	curtain = nullptr;
	goomba = nullptr;
	greenTurtoise = nullptr;
	star = nullptr;
	mushroom = nullptr;
	leaf = nullptr;
	bigBush = nullptr;
	menu = nullptr;

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

void CIntroScene::HandleMario()
{
	if (curtain->IsEnable == false && background->GetNameOfGame()->IsEnable == false)// rem vua cuon len
	{
		if(mario->state == MARIO_STATE_WALKING_LEFT && mario->vx == 0)
			mario->SetState(MARIO_STATE_WALKING_LEFT);
	}
	else if (leaf->IsEnable && leaf->y > 75) // start jump to reach up the leaf
	{
		if (mario->IsLookingUp)
		{
			mario->SetJumpStack(0);
			mario->IsReadyJump = true;
			mario->SetState(MARIO_STATE_JUMP);
			mario->UpJumpStack();
			mario->IsTouchingGround = false;
		}
		else if ( mario->GetJumpStack() < MARIO_MAX_JUMPIMG_STACKS) // High jump
		{
			mario->SetState(MARIO_STATE_JUMP);
			mario->UpJumpStack();
		}
	}
	else if (mario->GetLevel() == MARIO_LEVEL_RACCOON)
	{
		if (goomba->IsEnable && goomba->GetState() != GOOMBA_STATE_DIE_Y) // fall slowly to kill goomba
		{
			if (mario->GetState() == MARIO_STATE_JUMP)
			{
				mario->SetState(MARIO_STATE_WALKING_LEFT);
			}
			mario->SlowFall();
		}
		else if (goomba->GetState() == GOOMBA_STATE_DIE_Y && luigi->IsEnable == false && mario->IsTouchingGround && mario->y > 150 && mario->nx <0) // turn around after kill goomba
		{
			mario->SetState(MARIO_STATE_WALKING_RIGHT);
		}
		else if (mario->state == MARIO_STATE_WALKING_RIGHT && mario->x > 160)
		{
			mario->SetState(MARIO_STATE_IDLE);
		}
		else if (mario->state == MARIO_STATE_IDLE && mario->x > 160 && luigi->x <224)
		{
			mario->SetState(MARIO_STATE_WALKING_LEFT);
		}
		else if (mario->x < 96 && mario->state == MARIO_STATE_WALKING_LEFT && luigi->state == MARIO_STATE_IDLE) // start jump over turtoise
		{
			mario->SetJumpStack(0);
			mario->IsReadyJump = true;
			mario->SetState(MARIO_STATE_JUMP);
			mario->UpJumpStack();
			mario->IsTouchingGround = false;
		}
		else if (mario->GetJumpStack() < MARIO_MAX_JUMPIMG_STACKS/2 && mario->IsJumping && mario->IsTouchingGround == false && greenTurtoise->state != KOOPA_SMALL_STATE_IDLE) // jump over turtoise 
		{
			mario->SetState(MARIO_STATE_JUMP);
			mario->UpJumpStack();
		}
		else if (luigi->IsEnable && luigi->state == MARIO_STATE_IDLE && greenTurtoise->state == KOOPA_SMALL_STATE_IDLE && mario->x < 25)
		{
			mario->SetState(MARIO_STATE_WALKING_RIGHT);
			mario->IsReadyHolding = true;
		}
		else if (mario->x > 97 && mario->IsHolding)
		{
			mario->IsReadyHolding = false;
			mario->IsHolding = false;
			mario->StartKick();
			mario->SetState(MARIO_STATE_IDLE);
		}
	}
	else if(mario->GetLevel() == MARIO_LEVEL_BIG && greenTurtoise->x > mario->x && luigi->x > CGame::GetInstance()->GetScreenWidth())
	{
		mario->SetLevel(MARIO_LEVEL_SMALL);
	}
	else if (mario->GetLevel() == MARIO_LEVEL_SMALL)
	{
		if (greenTurtoise->IsEnable == false && mario->state == MARIO_STATE_IDLE)
			mario->SetState(MARIO_STATE_WALKING_RIGHT);
		else if (mario->state == MARIO_STATE_WALKING_RIGHT && mario->x > 230 && bigBush == nullptr)
			mario->SetState(MARIO_STATE_WALKING_LEFT);
		else if (mario->state == MARIO_STATE_WALKING_LEFT && mario->x < 155 && bigBush == nullptr)
		{
			mario->SetState(MARIO_STATE_WALKING_RIGHT);
			ShowBigBush();
		}
		else if (mario->x > 300 && mario->IsEnable)
		{
			mario->IsEnable = false;
			ShowMenu();
		}
	}

}
void CIntroScene::HandleLuigi()
{
	if (curtain->IsEnable == false && background->GetNameOfGame()->IsEnable == false)// pulling up done
	{
		if (luigi->state == MARIO_STATE_WALKING_RIGHT && luigi->vx == 0)
			luigi->SetState(MARIO_STATE_WALKING_RIGHT);
		else if (luigi->state == MARIO_STATE_WALKING_RIGHT && luigi->vx != 0) // calcutate jumping on mario's top
		{
			if (luigi->x > 55 && luigi->x < 60)		//start jump
			{
				if (luigi->IsTouchingGround == true)
				{
					luigi->SetJumpStack(0);
					luigi->IsReadyJump = true;
					luigi->SetState(MARIO_STATE_JUMP);
					luigi->UpJumpStack();
					luigi->IsTouchingGround = false;
				}
			}
		}
		else if (luigi->IsJumping && luigi->GetJumpStack() < MARIO_MAX_JUMPIMG_STACKS) // High jump
		{
			if (luigi->IsReadyJump == true && luigi->GetJumpStack() < MARIO_MAX_JUMPIMG_STACKS)
			{
				luigi->SetState(MARIO_STATE_JUMP);
				luigi->UpJumpStack();
			}
		}	 
		else if(luigi->y <0) //pull dơn NameOfGame 
		{
			ShowNameOfGame();
			mario->IsDucking = false;
			mario->y = mario->y + MARIO_BBOX_DUCKING_HEIGHT - MARIO_BIG_BBOX_HEIGHT;
		}
		//else if(luigi->x > 250)
	}
	else if (background->GetNameOfGame()->IsEnable == true)
	{
		if (background->GetNameOfGame()->GetSpriteId() == NAMEOFGAME_SPRITE_BLACK)
		{//Disable luigi
			if (luigi->x > CGame::GetInstance()->GetScreenWidth() + 20)
			{
				luigi->IsEnable = false;
				ShowStar();
				ShowGoomba();
				ShowLeaf();
				ShowMushroom();
			}
		}
		else if (mario->GetLevel() == MARIO_LEVEL_RACCOON)
		{
			if (mario->x > 160 && mario->vx == 0)
			{
				luigi->IsEnable = true;
				luigi->SetState(MARIO_STATE_WALKING_LEFT);
				luigi->IsReadyHolding = true;
			}
			else if (mario->state == MARIO_STATE_WALKING_LEFT && luigi->IsHolding && luigi->x < 224 && luigi->state == MARIO_STATE_WALKING_LEFT)
			{
				luigi->SetState(MARIO_STATE_IDLE);
			}
			else if (mario->x < 130 && mario->state == MARIO_STATE_WALKING_LEFT && luigi->state == MARIO_STATE_IDLE)
			{
				luigi->IsReadyHolding = false;
				luigi->IsHolding = false;
			}
			else if (mario->IsKicking && luigi->state == MARIO_STATE_IDLE)
			{
				luigi->SetState(MARIO_STATE_WALKING_RIGHT);
			}
			else if (luigi->state == MARIO_STATE_WALKING_RIGHT && luigi->x > CGame::GetInstance()->GetScreenWidth())
				luigi->IsEnable = false;
		}
	}

}
void CIntroScene::HandleGoomba()
{
	if (goomba->IsEnable)
	{
		if (goomba->state == GOOMBA_STATE_IDLE)
		{
			if (goomba->x + GOOMBA_BBOX_NORMAL_WIDTH + 5> mario->x)
				goomba->SetState(GOOMBA_STATE_WALKING);
		}
	}
}
void CIntroScene::HandleStar()
{
	if (star->IsEnable)
	{
		if (star->IsTouchingGround)
			star->SetState(STAR_STATE_JUMP_RIGHT);
		if (star->x > CGame::GetInstance()->GetScreenWidth())
			star->IsEnable = false;
	}
}
void CIntroScene::HandleTurtoise()
{
	if (greenTurtoise->IsEnable)
	{
		if (greenTurtoise->x > CGame::GetInstance()->GetScreenWidth() && mario->GetState() == MARIO_STATE_WALKING_RIGHT)
			greenTurtoise->SetState(KOOPA_SMALL_STATE_IDLE);
		else if (mario->GetLevel() == MARIO_LEVEL_RACCOON && mario->state == MARIO_STATE_IDLE && mario->nx > 0 && greenTurtoise->x > 300)
		{
			greenTurtoise->x = -50;
		}
		else if (mario->GetLevel() == MARIO_LEVEL_SMALL && greenTurtoise->x > 300)
			greenTurtoise->IsEnable = false;
	}
}
void CIntroScene::HandleMushroom()
{
	if (mushroom->IsEnable)
	{
		if (mushroom->IsTouchingGround && mushroom->vx == 0)
			mushroom->SetState(REWARD_LEVEL_UP_STATE_WALKING_LEFT);
	}
}
void CIntroScene::HandleLeaf()
{
	if (leaf->IsEnable)
	{
		if (leaf->y > 20 && greenTurtoise->IsEnable == false)
			ShowGreenTurtoise();
	}
}
void CIntroScene::ShowMario()
{
	mario->IsEnable = true;
}
void CIntroScene::ShowLuigi()
{
	luigi->IsEnable = true;
}
void CIntroScene::ShowNameOfGame()
{
	background->GetNameOfGame()->IsEnable = true;
}
void CIntroScene::ShowStar()
{
	star->IsEnable = true;
}
void CIntroScene::ShowGoomba()
{
	goomba->IsEnable = true;
}
void CIntroScene::ShowLeaf()
{
	leaf->IsEnable = true;
	leaf->SetState(REWARD_LEVEL_UP_STATE_SHAKING_RIGHT);
}
void CIntroScene::ShowGreenTurtoise()
{
	greenTurtoise->IsEnable = true;
	greenTurtoise->isTouchingGround = false;
	greenTurtoise->SetState(KOOPA_SMALL_STATE_IDLE);
}
void CIntroScene::ShowMushroom()
{
	mushroom->IsEnable = true;
}
void CIntroScene::ShowBigBush()
{
	bigBush = CSprites::GetInstance()->Get(BIGBUSH_SPRITE_ID);
}
void CIntroScene::ShowMenu()
{
	menu->IsEnable = true;
}






void CIntroScenceKeyHandler::KeyState(BYTE* states)
{

}
void CIntroScenceKeyHandler::OnKeyDown(int KeyCode)
{
	DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	CIntroScene* s =(CIntroScene*) CGame::GetInstance()->GetCurrentScene();
	CMenuIntro* menu = s->GetMenuIntro();
	switch (KeyCode)
	{
	case DIK_Q:
		if (menu->IsEnable)
		{
			if (menu->state == MENU_STATE_1PLAYER)
				menu->SetState(MENU_STATE_2PLAYER);
			else
				menu->SetState(MENU_STATE_1PLAYER);
		}
		break;
	case DIK_W:
		if (menu->IsEnable == false)
			s->ShowMenu();
		else
		{
			CGame::GetInstance()->SwitchScene(WORLDMAP_1_ID);
		}
		break;
	}
}
void CIntroScenceKeyHandler::OnKeyUp(int KeyCode)
{

}
