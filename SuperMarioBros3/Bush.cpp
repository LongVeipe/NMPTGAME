#include "Bush.h"


void CBush::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{

}
void CBush::Render()
{
	animation_set->at(0)->Render(x, y);
}
void CBush::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{

}