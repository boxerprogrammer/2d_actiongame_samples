#pragma once
#include "../Graphics/BaseDrawer.h"

///主にノベル用のテキスト表示を行うクラス
class TextDrawer : public BaseDrawer
{
private:
	int _colcount;
	int _rowcount;
	int _charcount;
	const char* _string;
	char _arrayForDisp[4];
	int _fontHeight;
public:
	TextDrawer(const char* string);
	~TextDrawer(void);
	void Draw();

};

