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
	Map(int idMap, int _tileWidth, int _tileHeight, int _tRTileSet, int	_tCTileSet, int	_tRMap, int	_tCMap, int	_totalTiles);
	~Map();
	void CreateTilesFromTileSet();
	void LoadMatrix(LPCWSTR path);
	void Render();
	void Draw(float x, float y);

	int GetTotalColsOfMap() { return this->TotalColsOfMap; }
	int GetTotalRowsOfMap() { return this->TotalRowsOfMap; }
	int GetTileWidth() { return this->TileWidth; }
	int GetTileHeight() { return this->TileHeight; }
};
