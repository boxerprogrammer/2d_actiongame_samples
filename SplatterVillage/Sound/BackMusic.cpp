#include "BackMusic.h"
#include <DxLib.h>



BackMusic::BackMusic(void)
{
}

BackMusic::BackMusic(const char* fileName){
	_handle = DxLib::PlayMusic(fileName,DX_PLAYTYPE_LOOP);
}


BackMusic::~BackMusic(void)
{
}


void
BackMusic::Play(){
	
}


void
BackMusic::Stop(){
}