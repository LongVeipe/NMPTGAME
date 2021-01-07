#pragma once

#include <map>
using namespace std;

class CZone
{
private:
	int left;
	int top;
	int right;
	int bottom;
public:
	CZone(int _left, int _top, int _right, int _bottom);
	int GetLeft() { return left; }
	int GetTop() { return top; }
	int GetRight() { return right; }
	int GetBottom() { return bottom; }
	void GetZone(int& _left, int& _top, int& _right, int& _bottom);
};

class CZones
{
	static CZones* __instance;
	map<int, CZone*> zones;

public:
	void Add(int id, CZone* zone);
	CZone* Get(int id);
	void Clear();

	static CZones* GetInstance();
};

