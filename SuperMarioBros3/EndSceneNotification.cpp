#include "EndSceneNotification.h"



CEndSceneNotification::CEndSceneNotification(float _x, float _y)
{
	x = _x;
	y = _y;
	font = new CFont();
	course = font->StringToSprites("COURSE");
	clear = font->StringToSprites("CLEAR");
}
void CEndSceneNotification::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

}
void CEndSceneNotification::Render()
{
	if (IsEnable)
	{
		for (unsigned int i = 0; i < course.size(); i++)
		{
			course.at(i)->Draw(x + i * FONT_WIDTH, y);
		}
		for (unsigned int i = 0; i < clear.size(); i++)
		{
			clear.at(i)->Draw(x + (i + course.size() + 1) * FONT_WIDTH, y);
		}
	}
}
void CEndSceneNotification::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{

}