#pragma once
#include "GameObject.h"
class CUnit
{
	friend class CGrid;
private:
	float x;
	float y;

	CGrid* grid;
	LPGAMEOBJECT obj;

	CUnit* prev;
	CUnit* next;
public:
	CUnit(CGrid* _grid, LPGAMEOBJECT _obj);
	CUnit(int gridRow, int gridCol, CGrid* _grid, LPGAMEOBJECT _obj);

	void Move(float _x, float _y);
	LPGAMEOBJECT GetObj() { return this->obj; }

};

class CGrid
{
	int mapWidth;
	int mapHeight;


	int numCols;
	int numRows;

	vector <vector<CUnit*>> cells;
public:
	CGrid(int _numRows, int _numCols);
	~CGrid();

	void Add(CUnit* unit);
	void Add(CUnit* unit, int gridRow, int gridCol);
	void Move(CUnit* unit, float x, float y);
	void Get(float cam_x, float cam_y, vector<CUnit*>& listUnits);
};