#include <fstream>
#include <iostream>
#include "Map.h"


Map::Map(LPDIRECT3DTEXTURE9 texture)
{
	this->TileSet = texture;
	/*this->TotalColsOfMap = _totalColsOfMap;
	this->TotalColsOfTileSet = _totalColsOfTileSet;
	this->TotalRowsOfMap = _totalRowsOfMap;
	this->TotalRowsOfTileSet = _totalRowsOfTileSet;
	this->TileHeight = _tileHeight;
	this->TileWidth = _tileWidth;
	this->TotalTiles = _totalTiles;
	this->Matrix = new int* [TotalRowsOfMap];
	for (int i = 0; i < TotalRowsOfMap; i++)
	{
		Matrix[i] = new int[TotalColsOfMap];
	}*/
}
Map::~Map()
{
	if (Matrix)
	{
		for (int i = 0; i < TotalRowsOfMap; i++)
		{
			delete Matrix[i];
		}
		delete Matrix;
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

void Map::Load(LPCWSTR path)
{
	ifstream f;

	f.open(path);
	//load map innfo
	f >> TileWidth >> TileHeight>> TotalRowsOfTileSet >> TotalColsOfTileSet >> TotalRowsOfMap >> TotalColsOfMap >> TotalTiles;
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

}

void Map::Render()
{
	for(int r = 0; r<this->TotalRowsOfMap; r++)
		for (int c = 0; c < this->TotalColsOfMap; c++)
		{
			Tiles[Matrix[r][c] - 1]->Draw(r * TileWidth, c * TileHeight, 255);
		}
}