#include <fstream>
#include <iostream>
#include "Map.h"
#include "Textures.h"
#include "PlayScence.h"


Map::Map(int _idMap, int _tileWidth, int _tileHeight, int _tRTileSet, int	_tCTileSet, int	_tRMap, int	_tCMap, int	_totalTiles)
{
	this->TileSet = CTextures::GetInstance()->Get(_idMap);
	this->TotalColsOfMap = _tCMap;
	this->TotalColsOfTileSet = _tCTileSet;
	this->TotalRowsOfMap = _tRMap;
	this->TotalRowsOfTileSet = _tRTileSet;
	this->TileHeight = _tileHeight;
	this->TileWidth = _tileWidth;
	this->TotalTiles = _totalTiles;
	this->MapHeight = this->MapWidth = 0;
}
Map::~Map()
{
	if (Matrix)
	{
		for (int i = 0; i < TotalRowsOfMap; i++)
		{
			delete [] Matrix[i];
		}
		delete [] Matrix;
		Matrix = nullptr;
	}
}

void Map::CreateTilesFromTileSet()
{
	int left, top, bottom, right;
	for (int i = 0; i < TotalTiles; i++)
	{
		left = (i % TotalColsOfTileSet * TileWidth);
		top = i / TotalColsOfTileSet * TileHeight;
		right = left + TileWidth;
		bottom = top + TileHeight;
		LPSPRITE Tile = new CSprite(i, left, top, right, bottom, TileSet);
		this->Tiles.push_back(Tile);
	}
}

void Map::LoadMatrix(LPCWSTR path)
{
  	ifstream f;

	f.open(path);
	//Init Matrix

	this->Matrix = new int* [TotalRowsOfMap];
	for (int i = 0; i < TotalRowsOfMap; i++)
	{
		Matrix[i] = new int[TotalColsOfMap];
		for (int j = 0; j < TotalColsOfMap; j++)
		{
   			f >> Matrix[i][j];
		}
	}
	f.close();
	this->MapHeight = TileHeight * TotalRowsOfMap;
	this->MapWidth = TileWidth * TotalColsOfMap;
}

void Map::Render()
{
	float cx, cy;
	CGame* game = CGame::GetInstance();
	game->GetCamPos(cx, cy);
	int startRow, startCol, limitRow, limitCol;
	startCol = cx / TileWidth;
	limitCol = startCol + (game->GetScreenWidth() / TileWidth)+1;

	if (limitCol >= TotalColsOfMap) limitCol = TotalColsOfMap - 1;

	for(int r = 0; r<TotalRowsOfMap; r++)
		for (int c = startCol; c < limitCol; c++)
		{
			Tiles[Matrix[r][c] - 1]->Draw(c * TileWidth,r * TileHeight, 255);
		}
}