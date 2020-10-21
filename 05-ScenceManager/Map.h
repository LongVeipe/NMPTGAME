#pragma once
#include <d3dx9.h>
#include <d3d9.h>
#include <vector>
#include "Sprites.h"

class Map
{
private:
	int** Matrix;
	int TotalColsOfTileSet, TotalRowsOfTileSet;
	int TotalColsOfMap, TotalRowsOfMap;
	int TotalTiles;
	int TileWidth, TileHeight;
	LPDIRECT3DTEXTURE9 TileSet;
	vector<LPSPRITE> Tiles;


public:
	Map(LPDIRECT3DTEXTURE9 texture, int _tileWidth, int _tileHeight, int _totalColsOfTileSet, int _totalRowsOfTileSet, int _totalColsOfMap, int _totalRowsOfMap, int _totalTiles);
	~Map();
	void CreateTilesFromTileSet();
	void LoadMatrix(int** _matrix);
	void Render();
	void Draw(float x, float y);

	int GetTotalColsOfMap() { return this->TotalColsOfMap; }
	int GetTotalRowsOfMap() { return this->TotalRowsOfMap; }
	int GetTileWidth() { return this->TileWidth; }
	int GetTileHeight() { return this->TileHeight; }
};
