#include "TextDrawer.h"
#include <DxLib.h>
namespace{
const int COLS=64;
const int ROWS=24;
char StringArray[ROWS][COLS]={
{'0','0','0','1','1','1','0','1','0','1','1','0','1','0','0','0','0','1','1','1','1','1','0','0','1','0','0','1','1','0','0','0','0','1','1','1','1','1','0','1','1','1','1','1','1','1','1','1','1','0','1','1','0','1','1','0','0','0','0','0','0','0','1','\0'},//1�s��
{'1','0','1','0','0','0','0','0','1','0','0','1','0','1','0','0','0','0','1','0','1','1','1','0','1','1','1','1','0','1','1','0','1','0','0','1','0','0','1','1','0','0','0','1','0','1','1','0','1','1','1','0','0','0','0','0','1','1','0','0','1','1','1','\0'},//2�s��
{'1','0','1','0','1','1','0','1','0','1','1','1','0','1','1','1','0','0','0','1','0','0','0','0','0','0','0','1','1','0','0','0','1','0','0','0','0','0','0','1','1','1','1','1','0','0','1','0','1','1','0','0','1','1','1','0','1','1','1','0','1','1','1','\0'},//3�s��
{'0','0','0','1','0','0','1','0','1','0','0','1','1','1','0','0','0','1','1','0','1','0','0','1','1','1','1','1','0','1','1','1','1','0','1','1','1','0','0','1','0','0','0','1','0','0','0','0','1','0','0','1','0','0','0','0','0','1','0','0','0','1','0','\0'},//4�s��
{'1','0','0','0','0','1','0','0','1','0','0','0','0','1','0','0','1','0','0','1','1','1','0','1','1','1','1','0','0','1','1','0','0','0','1','0','0','0','0','0','1','1','1','1','1','0','0','0','0','0','1','1','0','0','0','0','1','0','0','1','0','0','0','\0'},//5�s��
{'0','0','1','1','0','1','1','0','0','0','1','1','0','0','0','1','0','1','1','0','1','1','1','1','0','0','1','1','1','1','1','1','0','0','0','1','1','0','1','1','0','0','1','1','0','0','0','1','1','0','1','1','1','1','0','1','0','0','1','1','1','1','0','\0'},//6�s��
{'0','1','1','1','1','0','0','1','1','0','1','1','1','0','0','0','0','1','1','1','1','1','1','0','1','1','0','1','1','1','0','0','0','1','0','1','1','1','0','0','0','0','0','1','0','0','1','1','0','0','0','1','0','0','0','1','1','0','1','0','0','1','0','\0'},//7�s��
{'0','0','1','0','0','1','0','1','0','0','0','1','1','1','0','0','1','1','0','0','0','1','1','1','1','1','1','1','1','1','1','0','1','1','1','1','1','0','1','0','0','1','0','0','0','1','1','0','0','1','1','0','1','1','1','1','1','1','0','1','1','1','1','\0'},//8�s��
{'0','0','0','1','1','1','1','0','1','1','0','1','0','1','1','0','1','0','0','0','0','0','1','1','0','1','1','1','0','1','1','1','0','0','0','0','1','0','0','0','0','0','1','1','1','1','0','1','1','1','0','1','0','0','0','1','0','1','0','0','1','1','0','\0'},//9�s��
{'0','1','1','1','1','0','1','1','0','0','0','0','0','0','1','1','0','1','1','0','1','1','1','1','1','1','1','0','0','0','1','1','0','0','0','0','0','0','0','0','1','0','1','0','0','1','0','1','1','0','0','0','0','1','1','0','1','1','1','1','1','0','1','\0'},//10�s��
{'0','1','1','0','0','0','1','1','0','1','0','0','0','0','0','0','1','0','1','0','1','1','1','0','0','0','1','1','0','1','0','0','0','0','1','0','0','0','1','1','1','1','0','1','0','1','0','1','0','1','1','0','0','1','0','1','1','0','1','1','1','0','1','\0'},//11�s��
{'1','1','0','1','1','0','1','0','1','0','0','0','1','0','0','0','0','0','0','0','1','0','1','0','0','0','1','1','0','1','0','0','0','1','0','0','1','1','0','0','1','1','0','1','0','1','0','1','0','0','1','1','0','0','1','1','0','0','0','1','0','1','0','\0'},//12�s��
{'1','1','1','1','1','1','1','0','1','1','0','0','0','0','1','0','0','0','0','0','0','1','1','1','1','1','1','0','1','1','1','0','1','0','1','0','1','1','0','0','0','1','0','0','0','0','0','0','1','1','1','1','1','0','1','1','1','1','1','1','0','1','0','\0'},//13�s��
{'1','1','1','0','0','1','1','1','0','0','1','1','0','1','0','1','1','0','1','0','0','0','1','0','1','1','0','1','0','1','1','1','1','1','1','1','1','0','0','1','1','1','1','1','1','1','1','0','1','0','1','0','0','0','0','1','1','0','0','0','0','0','0','\0'},//14�s��
{'0','1','0','1','1','1','1','0','1','0','1','1','1','0','1','0','0','1','0','1','0','0','1','0','0','1','0','0','1','0','0','0','1','1','0','0','1','1','0','1','0','1','1','0','0','0','0','0','1','0','1','0','0','1','1','1','0','1','0','0','1','0','1','\0'},//15�s��
{'1','1','0','1','1','1','0','0','0','0','0','1','1','1','1','0','1','0','1','0','1','1','1','1','1','1','1','1','0','1','0','0','0','1','0','0','1','0','1','0','0','0','0','1','1','0','0','0','0','0','0','0','0','0','1','1','0','0','0','1','1','0','1','\0'},//16�s��
{'0','0','0','0','0','1','0','1','1','0','0','0','1','1','0','1','0','1','0','1','0','0','1','1','0','0','1','1','0','0','0','1','1','1','1','1','0','1','0','1','1','0','0','1','1','1','1','0','1','0','0','1','0','0','0','1','0','0','1','1','0','0','1','\0'},//17�s��
{'0','1','1','0','0','0','0','0','1','0','1','1','0','1','0','1','1','0','1','1','1','0','0','1','0','0','0','0','0','0','1','0','0','1','1','0','1','1','0','1','1','1','0','0','1','1','0','1','1','1','0','1','1','0','0','1','0','0','0','1','1','0','0','\0'},//18�s��
{'0','1','1','1','0','1','0','0','0','0','1','0','1','1','1','0','0','1','1','0','0','0','0','1','0','0','1','1','0','1','0','1','0','0','1','0','0','0','0','0','1','1','1','0','1','0','1','1','1','0','1','1','1','1','1','1','1','0','0','1','1','1','0','\0'},//19�s��
{'0','0','0','1','0','1','1','0','0','0','0','1','0','1','1','0','0','0','1','1','1','0','0','0','0','1','0','1','0','1','1','1','1','0','1','0','0','0','1','1','1','0','0','0','0','1','0','0','1','0','1','1','0','0','1','1','0','1','1','1','1','1','1','\0'},//20�s��
{'1','0','0','0','1','1','0','1','1','1','1','1','1','1','0','0','1','0','1','1','1','1','1','0','0','1','0','1','1','1','1','0','0','0','0','1','1','0','1','1','0','0','1','0','0','1','0','1','0','0','0','0','0','0','0','1','0','0','0','1','1','0','1','\0'},//21�s��
{'0','1','1','0','1','0','1','0','1','1','1','0','0','1','0','1','1','1','0','0','1','0','0','0','1','1','1','1','1','0','1','0','0','1','0','1','1','1','1','0','0','1','1','1','0','0','1','0','0','1','0','0','1','0','1','1','0','0','0','0','1','1','0','\0'},//22�s��
{'1','0','0','0','0','0','1','1','1','0','0','0','0','0','1','0','1','0','1','1','0','1','1','0','1','0','1','0','1','0','1','1','0','0','0','0','1','1','0','1','1','1','1','0','1','1','0','0','1','1','0','1','1','1','1','0','0','1','1','0','1','1','0','\0'},//23�s��
{'1','0','1','0','0','0','0','0','0','0','0','1','0','0','1','0','1','0','1','1','1','0','0','0','1','0','0','1','1','1','0','0','1','1','0','0','0','1','0','1','1','1','0','1','0','1','0','1','1','1','1','1','0','0','1','1','0','1','1','1','1','0','0','\0'},//24�s��
};



}

TextDrawer::TextDrawer(const char* string):_string(string),_rowcount(0),_colcount(0),_charcount(0)
{
	_fontHeight = DxLib::GetFontSize();
}


TextDrawer::~TextDrawer(void)
{
}

void
TextDrawer::Draw(){
	int y=0;
	static int a=0;
	if(a==0){
		switch(_string[_charcount]){
			case '\r':
				++_charcount;
			case '\n':
				++_charcount;
				_colcount=0;
				++_rowcount;
				StringArray[_rowcount][_colcount]='\0';
				break;
			default:
				StringArray[_rowcount][_colcount]=_string[_charcount];
				StringArray[_rowcount][_colcount+1]=_string[_charcount+1];
				StringArray[_rowcount][_colcount+2]='\0';
				_colcount+=2;
				_charcount+=2;
		}
	}
	a=(a+1)%3;
	for(int row=0;row<=_rowcount;++row){
		DxLib::DrawString(0+2,y+2,StringArray[row],0x000000);
		DxLib::DrawString(0,y,StringArray[row],0xffffff);
		y+=_fontHeight+4;
	}
}