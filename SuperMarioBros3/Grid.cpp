#include "Grid.h"
#include "Utils.h"

#define CELL_WIDTH	150
#define CELL_HEIGHT 150
#define SCREEN_WIDTH	270
#define SCREEN_HEIGHT	250

CUnit::CUnit(CGrid* _grid, LPGAMEOBJECT _obj)
{
	this->grid = _grid;
	this->obj = _obj;
	this->x = obj->x;
	this->y = obj->y;

	this->prev = NULL;
	this->next = NULL;

	grid->Add(this);
}

CUnit::CUnit(int _gridRow, int _gridCol,CGrid* _grid, LPGAMEOBJECT _obj)
{
	this->grid = _grid;
	this->obj = _obj;
	this->x = obj->x;
	this->y = obj->y;

	this->prev = NULL;
	this->next = NULL;

	grid->Add(this, _gridRow, _gridCol);
}

void CUnit::Move(float x, float y)
{
	grid->Move(this, x, y);

}




CGrid::CGrid(int gridRows, int gridCols)
{
	this->numCols = gridCols;
	this->numRows = gridRows;

	this->mapWidth = gridCols * CELL_WIDTH;
	this->mapHeight = gridRows * CELL_HEIGHT;


	cells.resize(numRows);

	for (int i = 0; i < numRows; i++)
		cells[i].resize(numCols);

	for (int i = 0; i < numRows; i++)
		for (int j = 0; j < numCols; j++)
			cells[i][j] = NULL;
}

CGrid::~CGrid()
{

}
void CGrid::Add(CUnit* unit)
{
	int row = (int)(unit->y / CELL_HEIGHT);
	int col = (int)(unit->x / CELL_WIDTH);

	if (row == this->numRows)
		row = this->numRows - 1;
	if (col == this->numCols)
		col = this->numCols - 1;

	unit->prev = NULL;
	unit->next = cells[row][col];
	cells[row][col] = unit;
	if (unit->next != NULL)
		unit->next->prev = unit;
}
void CGrid::Add(CUnit* unit, int gridRow, int gridCol)
{
	if (gridRow == this->numRows)
		gridRow = this->numRows - 1;
	if (gridCol == this->numCols)
		gridCol = this->numCols - 1;

	unit->prev = NULL;
	unit->next = cells[gridRow][gridCol];
	cells[gridRow][gridCol] = unit;
	if (unit->next != NULL)
		unit->next->prev = unit;
}

void CGrid::Move(CUnit* unit, float x, float y)
{
	int oldRow = (int)(unit->y / CELL_HEIGHT);
	int oldCol = (int)(unit->x / CELL_WIDTH);

	int newRow = (int)(y / CELL_HEIGHT);
	int newCol = (int)(x / CELL_WIDTH);

	if (newRow < 0 || newRow >= numRows || newCol < 0 ||
		newCol >= numCols)
	{
		return;
	}

	unit->x = x;
	unit->y = y;

	if (oldRow == newRow && oldCol == newCol)
		return;

	if (unit->prev != NULL)
	{
		unit->prev->next = unit->next;
	}
	if (unit->next != NULL)
	{
		unit->next->prev = unit->prev;
	}
	if (cells[oldRow][oldCol] == unit)
		cells[oldRow][oldCol] = unit->next;

	Add(unit);
}

void CGrid::Get(float cam_x, float cam_y, vector<CUnit*>& listUnits)
{
	int startCol = (int)(cam_x / CELL_WIDTH);
	int endCol = (int)ceil((cam_x + SCREEN_WIDTH) / CELL_WIDTH);
	int ENDCOL = (int)ceil((mapWidth) / CELL_WIDTH);
	if (endCol > ENDCOL)
		endCol = ENDCOL;

	int startRow = (int)(cam_y / CELL_HEIGHT - 1);
	if (startRow < 0)
		startRow = 0;

	int endRow = (int)ceil((cam_y + SCREEN_HEIGHT / CELL_HEIGHT +1));
	int ENDROW = (int)ceil((mapHeight / CELL_HEIGHT));
	if (endRow > ENDROW)
		endRow = ENDROW;
	for (int i = startRow; i < endRow; i++)
	{
		for (int j = startCol; j < endCol; j++)
		{
			CUnit* unit = cells[i][j];
			while (unit != NULL)
			{
				if (unit->GetObj()->IsInCamera())
					listUnits.push_back(unit);
				unit = unit->next;
			}
		}
	}
}

