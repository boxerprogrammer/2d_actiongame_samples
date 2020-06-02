#include "BeforeBattle.h"
#include"GameMain.h"
#include"BattleScene.h"
#include<DxLib.h>

const int FADEIN_INTERVAL = 30;
const int FADEOUT_INTERVAL = 120;

BeforeBattle::BeforeBattle(int playerno,int enemyno):_playerno(playerno),_enemyno(enemyno)
{
	_updater = &BeforeBattle::FadeInUpdate;
	_beforeBattleH=DxLib::LoadGraph("img/beforebattle.png");
	_bgmH = DxLib::LoadSoundMem("bgm/beforebattle.wav");
	_doraH = DxLib::LoadSoundMem("bgm/dora.mp3");
	_pixelArtH = DxLib::LoadGraph("img/yie_ar_kung_fu.png");
	_fadeTimer = 0;
	_timer = 0;
}


BeforeBattle::~BeforeBattle()
{
	DxLib::DeleteSoundMem(_bgmH);
	DxLib::DeleteSoundMem(_doraH);
	DxLib::DeleteGraph(_beforeBattleH);
	DxLib::DeleteGraph(_pixelArtH);
}


void 
BeforeBattle::FadeInUpdate(const KeyState& key) {
	GameMain& gm = GameMain::Instance();
	DxLib::DrawGraph(0, 0, _beforeBattleH, false);
	int brightness = (255 * _fadeTimer) / FADEIN_INTERVAL;
	DxLib::SetDrawBlendMode(DX_BLENDMODE_MUL, 0);
	DxLib::DrawBox(0, 0, gm.WindowSize().w, gm.WindowSize().h, GetColor(brightness, brightness, brightness), true);
	DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	if (++_fadeTimer == FADEIN_INTERVAL) {
		_updater = &BeforeBattle::NormalUpdate;
		DxLib::PlaySoundMem(_bgmH, DX_PLAYTYPE_BACK);
	}
}
void 
BeforeBattle::NormalUpdate(const KeyState& key) {
	GameMain& gm = GameMain::Instance();
	int idx = (_timer / 30);
	_timer = (_timer + 1) % 60;
	DxLib::DrawGraph(0, 0, _beforeBattleH, false);
	DxLib::DrawBox(136, 172, 136 + 80, 172 + 80,0x00000000,true);
	DxLib::DrawRectExtendGraphF(136, 172, 136 + 80, 172 + 80, 176+idx*40, 128,40,40 , _pixelArtH, true);
	if (DxLib::CheckSoundMem(_bgmH) == 0) {
		_updater = &BeforeBattle::FadeOutUpdate;
		DxLib::PlaySoundMem(_doraH, DX_PLAYTYPE_BACK);
		_fadeTimer = FADEOUT_INTERVAL;
	}
}
void 
BeforeBattle::FadeOutUpdate(const KeyState& key) {
	GameMain& gm = GameMain::Instance();
	DxLib::DrawGraph(0, 0, _beforeBattleH, false);
	int brightness = (255 * _fadeTimer) / FADEOUT_INTERVAL;
	DxLib::SetDrawBlendMode(DX_BLENDMODE_MUL, 0);
	DxLib::DrawBox(0, 0, gm.WindowSize().w, gm.WindowSize().h, GetColor(brightness, brightness, brightness), true);
	DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	_fadeTimer = max(_fadeTimer - 1, 0);
	if (_fadeTimer <= 0 ) {
		gm.ChangeScene(new BattleScene(_playerno,_enemyno));
	}
}

void 
BeforeBattle::Update(const KeyState& key) {
	(this->*_updater)(key);
}