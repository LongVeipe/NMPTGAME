#include "Font.h"

CFont::CFont()
{
	CSprites* sprites = CSprites::GetInstance();
	fonts.insert(make_pair('0', sprites->Get(FONT_SPRITE_0)));
	fonts.insert(make_pair('1', sprites->Get(FONT_SPRITE_1)));
	fonts.insert(make_pair('2', sprites->Get(FONT_SPRITE_2)));
	fonts.insert(make_pair('3', sprites->Get(FONT_SPRITE_3)));
	fonts.insert(make_pair('4', sprites->Get(FONT_SPRITE_4)));
	fonts.insert(make_pair('5', sprites->Get(FONT_SPRITE_5)));
	fonts.insert(make_pair('6', sprites->Get(FONT_SPRITE_6)));
	fonts.insert(make_pair('7', sprites->Get(FONT_SPRITE_7)));
	fonts.insert(make_pair('8', sprites->Get(FONT_SPRITE_8)));
	fonts.insert(make_pair('9', sprites->Get(FONT_SPRITE_9)));
	fonts.insert(make_pair('A', sprites->Get(FONT_SPRITE_A)));
	fonts.insert(make_pair('B', sprites->Get(FONT_SPRITE_B)));
	fonts.insert(make_pair('C', sprites->Get(FONT_SPRITE_C)));
	fonts.insert(make_pair('D', sprites->Get(FONT_SPRITE_D)));
	fonts.insert(make_pair('E', sprites->Get(FONT_SPRITE_E)));
	fonts.insert(make_pair('F', sprites->Get(FONT_SPRITE_F)));
	fonts.insert(make_pair('G', sprites->Get(FONT_SPRITE_G)));
	fonts.insert(make_pair('H', sprites->Get(FONT_SPRITE_H)));
	fonts.insert(make_pair('I', sprites->Get(FONT_SPRITE_I)));
	fonts.insert(make_pair('J', sprites->Get(FONT_SPRITE_J)));
	fonts.insert(make_pair('K', sprites->Get(FONT_SPRITE_K)));
	fonts.insert(make_pair('L', sprites->Get(FONT_SPRITE_L)));
	fonts.insert(make_pair('M', sprites->Get(FONT_SPRITE_M)));
	fonts.insert(make_pair('N', sprites->Get(FONT_SPRITE_N)));
	fonts.insert(make_pair('O', sprites->Get(FONT_SPRITE_O)));
	fonts.insert(make_pair('P', sprites->Get(FONT_SPRITE_P)));
	fonts.insert(make_pair('Q', sprites->Get(FONT_SPRITE_Q)));
	fonts.insert(make_pair('R', sprites->Get(FONT_SPRITE_R)));
	fonts.insert(make_pair('S', sprites->Get(FONT_SPRITE_S)));
	fonts.insert(make_pair('T', sprites->Get(FONT_SPRITE_T)));
	fonts.insert(make_pair('U', sprites->Get(FONT_SPRITE_U)));
	fonts.insert(make_pair('V', sprites->Get(FONT_SPRITE_V)));
	fonts.insert(make_pair('W', sprites->Get(FONT_SPRITE_W)));
	fonts.insert(make_pair('X', sprites->Get(FONT_SPRITE_X)));
	fonts.insert(make_pair('Y', sprites->Get(FONT_SPRITE_Y)));
	fonts.insert(make_pair('Z', sprites->Get(FONT_SPRITE_Z)));
}

LPSPRITE CFont::mapping(char c)
{
	int asciiCode = (int)c;
	if ((asciiCode >= 48 && asciiCode <= 57) || (asciiCode >= 65 && asciiCode <= 90))
		return fonts.at(c);
	else
		return nullptr;
}

vector<LPSPRITE> CFont::StringToSprites(string str)
{
	vector<LPSPRITE> sprites;
	LPSPRITE sprite;
	for (unsigned int i = 0; i < str.size(); i++)
	{
		char c = (char)str[i];
		sprite = mapping(c);
		if(sprite != nullptr)
			sprites.push_back(sprite);
	}
	return sprites;
}
CFont* CFont::__instance = nullptr;

CFont* CFont::GetInstance()
{
	if (__instance == nullptr) 
		__instance = new CFont();
	return __instance;
}
