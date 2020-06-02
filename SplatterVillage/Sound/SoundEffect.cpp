#include "SoundEffect.h"
#include <DxLib.h>

SoundEffect::SoundEffect(void)
{
}


SoundEffect::~SoundEffect(void)
{
}

SoundEffect::SoundEffect(const char* filename){
	Load(filename);
}

void 
SoundEffect::Load(const char* filename){
	_handle = DxLib::LoadSoundMem(filename);
}

void 
SoundEffect::Play(){
	DxLib::PlaySoundMem(_handle,DX_PLAYTYPE_BACK);
}





