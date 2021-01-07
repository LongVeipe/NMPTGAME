#include "Zone.h"
#include "Utils.h"

CZone::CZone(int _left, int _top, int _right, int _bottom)
{
	left = _left;
	top = _top;
	right = _right;
	bottom = _bottom;
}

void CZone::GetZone(int& _left, int& _top, int& _right, int& _bottom)
{
	_left = left;
	_top = top;
	_right = right;
	_bottom = bottom;
}

CZones* CZones::__instance = nullptr;

void CZones::Add(int id, CZone* zone)
{
	zones[id] = zone;
	DebugOut(L"[INFO] Zones added: %d \n", id);
}

CZone* CZones::Get(int id)
{
	CZone* zone = zones[id];
	if (zone == NULL)
		DebugOut(L"[ERROR] Failed to find zone id: %d\n", id);
	return zone;
}
void CZones::Clear()
{
	for (unsigned int i = 0; i< zones.size(); i++)
	{
		CZone* zone = zones[i];
		delete zone;
	}
	zones.clear();
}
CZones* CZones::GetInstance()
{
	if (__instance == nullptr)
		__instance = new CZones();
	return __instance;
}