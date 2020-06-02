#include "BGMManager.h"
#include <DxLib.h>

const char* BGMTable[]={"snd/bgm/title.mp3",
			"snd/bgm/opening1.mp3",
			"snd/bgm/opening2.mp3",
			"snd/bgm/before_stage.mp3",
			"snd/bgm/stage_1.mp3"};

const int _defaultFadeSpan=60;//60フレーム
const int _defaultVolume=255;//デフォルトボリューム


BGMManager::BGMManager(void)
{
	_updater=&BGMManager::Normal;
}


BGMManager::~BGMManager(void)
{
}


void
BGMManager::Play(const char* filename){
	DxLib::PlayMusic(filename,DX_PLAYTYPE_LOOP);
	_currentVolume = _defaultVolume;
	DxLib::SetVolumeMusic(_currentVolume);
}

void
BGMManager::Play(BGMID bgmid){
	Play(BGMTable[bgmid]);
}

void
BGMManager::Stop(){
	DxLib::StopMusic();
}

void
BGMManager::FadeOut(const int frame){
	_updater=&BGMManager::Fade;
	_reduction = _currentVolume / frame; 
}

void
BGMManager::Update(){
	(this->*_updater)();
}

void
BGMManager::Normal(){
	//なにもしません
}

void
BGMManager::Fade(){
	if(_currentVolume<=0){
		Stop();
		_updater=&BGMManager::Normal;
		_reduction=0;

	}else{
		DxLib::SetVolumeMusic(_currentVolume);
		_currentVolume-=_reduction;
	}
}
