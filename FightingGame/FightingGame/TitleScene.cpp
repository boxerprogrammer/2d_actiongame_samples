#include "TitleScene.h"
#include"GameMain.h"//切り替え関数を呼び出すのに必要
#include"CharacterSelectScene.h"//次シーン
#include<DxLib.h>//色々処理するために必要

const int FADEIN_INTERVAL = 60;
const int FADEOUT_INTERVAL = 30;
const int MAX_BRIGHTNESS = 255;
int maskH;
int menuBackH;
TitleScene::TitleScene()
{
	CreateMaskScreen();
	SetUseMaskScreenFlag(false);
	_startSE = DxLib::LoadSoundMem("se/start_se.mp3");
	_titleH = DxLib::LoadGraph("img/title.png");
	_pressStartH = DxLib::LoadGraph("img/press_start.png");
	maskH = DxLib::LoadMask("img/menu_back_mask.png");
	menuBackH = DxLib::LoadGraph("img/menu_back_over.png");
	DrawMask(0,0,maskH, DX_MASKTRANS_BLACK);
	_updater = &TitleScene::FadeInUpdate;
	_fadeTimer = 0;
}


TitleScene::~TitleScene()
{
	DeleteGraph(_titleH);
	DeleteGraph(_pressStartH);
	DeleteSoundMem(_startSE);
}


void
TitleScene::Update(const KeyState& key) {
	(this->*_updater)(key);
}


void 
TitleScene::FadeInUpdate(const KeyState& key) {
	GameMain& gm = GameMain::Instance();
	int brightness = (MAX_BRIGHTNESS * _fadeTimer) / FADEIN_INTERVAL;
	DxLib::DrawGraph(0, 0, _titleH, false);
	DxLib::SetDrawBlendMode(DX_BLENDMODE_MUL, 0);
	DxLib::DrawBox(0, 0, gm.WindowSize().w, gm.WindowSize().h, GetColor(brightness, brightness, brightness), true);
	DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	if (++_fadeTimer == FADEIN_INTERVAL) {
		_updater = &TitleScene::NormalUpdate;
	}
}

void 
TitleScene::NormalUpdate(const KeyState& key) {
	GameMain& gm = GameMain::Instance();
	DxLib::DrawGraph(0, 0, _titleH, false);
	_blinkTimer = (_blinkTimer + 1) % 60;

	if (_blinkTimer / 30 == 0) {
		int w, h;
		GetGraphSize(_pressStartH, &w, &h);
		DxLib::DrawGraph((gm.WindowSize().w - w) / 2, 380, _pressStartH, true);
	}
	if (key.state[KEY_INPUT_RETURN] && !key.lastState[KEY_INPUT_RETURN]) {
		_updater = &TitleScene::FastBlinkUpdate;
		_waitTimer = 30;
		DxLib::PlaySoundMem(_startSE, DX_PLAYTYPE_BACK);
	}
	static float angle=0.0f;
	SetUseMaskScreenFlag(true);
	DrawRotaGraph2(320,240,370,370,1.0,angle,menuBackH,true);
	DrawRotaGraph2(320, 240, 370, 370, 1.0, angle+3.14f, menuBackH, true);
	SetUseMaskScreenFlag(false);
	angle+=0.01;
}
void 
TitleScene::FastBlinkUpdate(const KeyState& key) {
	GameMain& gm = GameMain::Instance();
	_blinkTimer = (_blinkTimer + 1) % 10;
	DxLib::DrawGraph(0, 0, _titleH, false);
	if (_blinkTimer / 5 == 1) {
		int w, h;
		GetGraphSize(_pressStartH, &w, &h);
		DxLib::DrawGraph((gm.WindowSize().w - w) / 2, 380, _pressStartH, true);
	}
	if (--_waitTimer == 0) {
		_updater = &TitleScene::FadeOutUpdate;
		_fadeTimer = FADEOUT_INTERVAL;
	}
}
void 
TitleScene::FadeOutUpdate(const KeyState& key) {
	GameMain& gm = GameMain::Instance();
	int brightness = (MAX_BRIGHTNESS * _fadeTimer) / FADEOUT_INTERVAL;
	DxLib::DrawGraph(0, 0, _titleH, false);
	DxLib::SetDrawBlendMode(DX_BLENDMODE_MUL, 255);
	DxLib::DrawBox(0, 0, gm.WindowSize().w, gm.WindowSize().h, GetColor(brightness, brightness, brightness), true);
	DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	if (--_fadeTimer == 0) {
		gm.ChangeScene(new CharacterSelectScene());
	}
	
}
