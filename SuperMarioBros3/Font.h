#pragma once
#include "Sprites.h"
#include <map>
using namespace std;

#define FONT_SPRITE_0	999101
#define FONT_SPRITE_1	999102
#define FONT_SPRITE_2	999103
#define FONT_SPRITE_3	999104
#define FONT_SPRITE_4	999105
#define FONT_SPRITE_5	999106
#define FONT_SPRITE_6	999107
#define FONT_SPRITE_7	999108
#define FONT_SPRITE_8	999109
#define FONT_SPRITE_9	999110
#define FONT_SPRITE_A	999111
#define FONT_SPRITE_B	999112
#define FONT_SPRITE_C	999113
#define FONT_SPRITE_D	999114
#define FONT_SPRITE_E	999115
#define FONT_SPRITE_F	999116
#define FONT_SPRITE_G	999117
#define FONT_SPRITE_H	999118
#define FONT_SPRITE_I	999119
#define FONT_SPRITE_J	999120
#define FONT_SPRITE_K	999121
#define FONT_SPRITE_L	999122
#define FONT_SPRITE_M	999123
#define FONT_SPRITE_N	999124
#define FONT_SPRITE_O	999125
#define FONT_SPRITE_P	999126
#define FONT_SPRITE_Q	999127
#define FONT_SPRITE_R	999128
#define FONT_SPRITE_S	999129
#define FONT_SPRITE_T	999130
#define FONT_SPRITE_U	999131
#define FONT_SPRITE_V	999132
#define FONT_SPRITE_W	999133
#define FONT_SPRITE_X	999134
#define FONT_SPRITE_Y	999135
#define FONT_SPRITE_Z	999136

#define FONT_WIDTH	8

class CFont
{
private:
	static CFont* __instance;
	map<char, LPSPRITE> fonts;
public:
	CFont();
	LPSPRITE mapping(char c);
	vector<LPSPRITE> StringToSprites(string str);

	static CFont* GetInstance();
};

