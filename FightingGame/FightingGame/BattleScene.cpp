#include "BattleScene.h"
#include"ContinueScene.h"
#include"EndingScene.h"
#include"GameMain.h"
#include"Player.h"
#include"Hadoken.h"
#include"Enemy.h"
#include"Star.h"
#include"CollisionDetector.h"
#include<DxLib.h>
#include<algorithm>

Vector2 playerVs[2] = { { 244,48 } ,{ 216,48 } };
Vector2 enemyVs[2] = { { 366,49 } ,{ 393,49 } }; 

int _playerRedGauge;
int _enemyRedGauge;

int kemogaugeH;
int kemogaugeM;

BattleScene::BattleScene(unsigned int playerno,unsigned int enemyno,unsigned int playerwin,unsigned int enemywin):_fadeTimer(0),_playerwin(playerwin),_enemywin(enemywin),_playerno(playerno),_enemyno(enemyno)
{
	_healthbarH = DxLib::LoadGraph("img/healthbar.png");
	_barH = DxLib::LoadGraph("img/bar.png");

	_pixelArtH = DxLib::LoadGraph("img/yie_ar_kung_fu.png");
	_stageRyuSky = DxLib::LoadGraph("img/ryu-sky.png");
	_stageRyuRoof = DxLib::LoadGraph("img/ryu-roof.png");
	_stageRyuTemple = DxLib::LoadGraph("img/ryu-temple.png");
	_kungfuBattleBGM = DxLib::LoadBGM("bgm/kungfubattle.wav");

	_hitEffectH = DxLib::LoadGraph("img/hiteffect.png");
	_bloodSplashH = DxLib::LoadGraph("img/bloodsplash.png");//2560*768=10枚*3枚
	
	_vH = DxLib::LoadGraph("img/v.png");
	_faceH = DxLib::LoadGraph("img/face.png");
	_knockOutH=DxLib::LoadGraph("img/ko.png");//
	_knockOutVoiceH = DxLib::LoadSoundMem("se/ko.wav");//

	_bloodTurn = false;
	_player = new Player(_pixelArtH);
	_enemy = new Star(_pixelArtH,_player);
	_updater = &BattleScene::FadeInUpdate;

	_hitEffectFrame = 0;
	_bloodSplashFrame = 0;

	const float gaugeWidthRatio = 270.0f / 100.0f;
	_playerRedGauge = _player->GetLife()*gaugeWidthRatio;
	_enemyRedGauge = _enemy->GetLife()*gaugeWidthRatio;
	kemogaugeH = LoadGraph("img/kemogauge.png");
	kemogaugeM = LoadGraph("img/kemogauge_mask.png");
}


BattleScene::~BattleScene()
{
	delete _player;
	delete _enemy;
	DxLib::DeleteGraph(_pixelArtH);
	DxLib::DeleteGraph(_stageRyuRoof);
	DxLib::DeleteGraph(_stageRyuSky);
	DxLib::DeleteGraph(_stageRyuTemple);
	DxLib::DeleteGraph(_hitEffectH);
	DxLib::DeleteGraph(_bloodSplashH);
	DxLib::DeleteGraph(_knockOutH);
	DxLib::DeleteGraph(_vH);
	DxLib::DeleteGraph(_faceH);

	DxLib::DeleteSoundMem(_kungfuBattleBGM);
	DxLib::DeleteSoundMem(_knockOutVoiceH);
	DxLib::DeleteGraph(_healthbarH);
	DxLib::DeleteGraph(_barH);

}

unsigned int LifeToColor(int life) {
	int colife = life*(512.0f / 100.0f);

	unsigned int R = min(max((384 - colife), 0),0xff);
	unsigned int G = min(max((colife+64), 0),0xff);
	unsigned int B = max((colife - 384), 0);
	return ((R & 0xff) << 16) | ((G & 0xff) << 8) | (B & 0xff);
}

void
BattleScene::DrawGauge() {
	const float gaugeWidthRatio= 270.0f / 100.0f;
	int barwidthR = max(_enemy->GetLife(), 0)*gaugeWidthRatio;
	DrawGraph(360,0,kemogaugeH,false);
	SetDrawBlendMode(DX_BLENDMODE_MUL, 255);
	//DxLib::DrawBox(360+barwidthR+3, 4, 360 + 278, 18, 0x707070, true);
	DxLib::DrawRectGraph(360+ barwidthR + 3, 4, barwidthR+3,0, 276- barwidthR-3, 24, kemogaugeM, false,false);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//ポトレ(プレイヤー)
	Position2 pportp = Position2(_playerno % 5, _playerno / 5);
	pportp.x *= 120;
	pportp.y *= 150;
	DxLib::DrawRectRotaGraph(30, 30+20, pportp.x, pportp.y, 120, 120, 0.4f, 0.f, _faceH, true, true);

	//ポトレ(敵)
	Position2 eportp = Position2(_enemyno % 5, _enemyno / 5);
	eportp.x *= 120;
	eportp.y *= 150;
	DxLib::DrawRectRotaGraph(610, 30 + 20, eportp.x,eportp.y, 120, 120, 0.4f, 0.f, _faceH, true, false);

	//体力バーの「ガワ」
	DxLib::DrawRotaGraph2(0, 15, 0, 0, 640.f/380.f, 0, _healthbarH, true, false);

	//赤ゲージ右
	int redwidthR = max(_enemyRedGauge, 0);
	DxLib::DrawBox(360, 25, 360 + redwidthR, 25 + 9, 0xff0000, true);

	//赤ゲージ左
	int redwidthL = max(_playerRedGauge, 0);
	DxLib::DrawBox(280 - redwidthL, 25, 280, 25 + 9, 0xff0000, true);

	//体力バー右
	/*int barwidthR = max(_enemy->GetLife(),0)*gaugeWidthRatio;*/
	DxLib::DrawExtendGraph(360, 25, 360 + barwidthR, 25 + 9, _barH, false);

	//体力バー左
	int barwidthL = max(_player->GetLife(),0)*gaugeWidthRatio;
	DxLib::DrawExtendGraph(280 - barwidthL, 25, 280, 25 + 9, _barH, false);
	
	unsigned int playerColor = LifeToColor(_player->GetLife());
	unsigned int enemyColor = LifeToColor(_enemy->GetLife());
	SetDrawBlendMode(DX_BLENDMODE_MUL, 255);
	DxLib::DrawBox(360, 25, 360 + barwidthR, 25 + 9, enemyColor, true);
	DxLib::DrawBox(280 - barwidthL, 25, 280, 25 + 9, playerColor, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);



	_playerRedGauge = max( _playerRedGauge-1,_player->GetLife()*gaugeWidthRatio);
	_enemyRedGauge = max( _enemyRedGauge-1, _enemy->GetLife()*gaugeWidthRatio);

	for (int i = 0; i < _playerwin; ++i) {
		DrawGraph(playerVs[i].x, playerVs[i].y, _vH, true);
	}
	for (int i = 0; i < _enemywin; ++i) {
		DrawGraph(enemyVs[i].x, enemyVs[i].y, _vH, true);
	}
	
}

void 
BattleScene::DrawBase() {
	Size roofGSize;
	GameMain& gm = GameMain::Instance();
	const Size& windowSize = gm.WindowSize();

	DxLib::DrawRotaGraph2(0, 0, 0, 0, 1.5f, 0, _stageRyuSky, false);//空(最背面)
	DxLib::DrawRotaGraph2(-320, 0, 0, 0, 1.5f, 0, _stageRyuTemple, true);//お城？

	DrawGauge();

	GetGraphSize(_stageRyuRoof, &roofGSize.w, &roofGSize.h);
	DxLib::DrawRotaGraph2(0, windowSize.h - (int)(roofGSize.h*1.2f), 0, 0, 1.5, 0, _stageRyuRoof, true);//屋根
}
void
BattleScene::FadeInUpdate(const KeyState& key) {

	GameMain& gm = GameMain::Instance();
	int brightness = (255 * _fadeTimer) / 30;
	
	DrawBase();

	DxLib::SetDrawBlendMode(DX_BLENDMODE_MUL, 0);
	DxLib::DrawBox(0, 0, gm.WindowSize().w, gm.WindowSize().h, GetColor(brightness, brightness, brightness), true);
	DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	
	if (++_fadeTimer == 30) {
		_updater = &BattleScene::NormalUpdate;
		DxLib::PlaySoundMem(_kungfuBattleBGM, DX_PLAYTYPE_LOOP);
	}
}

void
BattleScene::NormalUpdate(const KeyState& key) {
	GameMain& gm = GameMain::Instance();
	DrawBase();
	_enemy->Update();
	
	//敵飛び道具と自機当たり判定
	for (auto& m : _enemy->GetMissiles()) {
		if (m.IsDead())continue;
		m.Update();
		const Rect& rc = m.GetRect().GetRect();
		if (CollisionDetector::IsHit(rc, _player->GetRect())) {
			m.Kill();
			_player->OnCollided(rt_attack);
			if (_player->GetLife() <= 0) {
				_hitEffectPos = CollisionDetector::OverlappedCenter(_player->GetRect(), rc);
				_bloodTurn = _hitEffectPos.x < _player->GetRect().pos.x;
				_bloodSplashFrame = 60;
				_player->PauseForRendering();
				_enemy->PauseForRendering();
				++_enemywin;
			}else {
				_hitEffectPos = CollisionDetector::OverlappedCenter(_player->GetRect(), rc);
				_hitEffectFrame = 45;
			}
		}
	}

	//自分の飛び道具と敵当たり判定
	auto hadoken = _player->GetMissile();
	if(!hadoken->IsDead()){
		const Rect& hadorc = hadoken->GetRect().GetRect();
		if (CollisionDetector::IsHit(_enemy->GetRect(), hadorc)) {
			hadoken->Kill();
			_enemy->OnCollided(rt_attack);
			if (_enemy->GetLife() <= 0) {
				_hitEffectPos = CollisionDetector::OverlappedCenter(_enemy->GetRect(), hadorc);
				_bloodTurn = _hitEffectPos.x < _enemy->GetRect().pos.x;
				_bloodSplashFrame = 60;
				_player->PauseForRendering();
				_enemy->PauseForRendering();
				++_playerwin;
			}else {
				_hitEffectPos = CollisionDetector::OverlappedCenter(_enemy->GetRect(), hadorc);
				_hitEffectFrame = 45;
			}
		}
	}

	_player->Update(key);

	//AI用
	if (CollisionDetector::IsHit(_enemy->GetRect(), _player->GetRect())) {
		_enemy->OnCollided(rt_none);
	}

	auto playerRects = _player->GetCurrentRects();
	if (playerRects != nullptr) {
		for (auto& rc : (*playerRects)) {
			if ( rc.GetType() == rt_attack) {
				if (!rc.IsDead() && CollisionDetector::IsHit(_enemy->GetRect(), rc.GetRect())) {
					_enemy->OnCollided(rc.GetType());
					_player->OnCollided(rt_damage);
					rc.Kill();
					if (_enemy->GetLife() <= 0) {
						_hitEffectPos = CollisionDetector::OverlappedCenter(_enemy->GetRect(), rc.GetRect());
						_bloodTurn = _hitEffectPos.x < _enemy->GetRect().pos.x;
						_bloodSplashFrame = 60;
						_player->PauseForRendering();
						_enemy->PauseForRendering();
						++_playerwin;
					}else{
						_hitEffectPos = CollisionDetector::OverlappedCenter(_enemy->GetRect(), rc.GetRect());
						_hitEffectFrame = 45;
					}
				}
			}
		}
	}
	if (_bloodSplashFrame > 0) {
		_bloodSplashFrame = max(_bloodSplashFrame - 1, 0);
		int srcX = (((60 - _bloodSplashFrame) / 2) % 10) * 256;
		int srcY = (((60 - _bloodSplashFrame) / 2) / 10) * 256;
		int cx = _bloodTurn ? 256 - 10 : 10;
		DxLib::DrawRectRotaGraph2(_hitEffectPos.x * 2, _hitEffectPos.y * 2, srcX, srcY, 256,256,cx, 206, 1.0f, 0.0, _bloodSplashH, true, _bloodTurn);
	}
	else {
		if (_hitEffectFrame > 0) {
			_hitEffectFrame = max(_hitEffectFrame - 1, 0);
			int srcX = (((45 - _hitEffectFrame) / 3) % 8) * 300;
			int srcY = (((45 - _hitEffectFrame) / 3) / 8) * 300;
			DxLib::DrawRectRotaGraph(_hitEffectPos.x * 2, _hitEffectPos.y * 2, srcX, srcY, 300, 300, 0.3, 0.0, _hitEffectH, true, false);
		}
	}
	if (_bloodSplashFrame == 0 && (_player->GetLife() <= 0 || _enemy->GetLife() <= 0)) {
		_updater = &BattleScene::KnockOutUpdate;
		PlaySoundMem(_knockOutVoiceH, DX_PLAYTYPE_BACK);
		_kotimer = 60;
	}
}


void
BattleScene::KnockOutUpdate(const KeyState& key) {
	GameMain& gm = GameMain::Instance();
	DrawBase();
	_enemy->Update();
	_player->Update(key);
	int w, h;
	DxLib::GetGraphSize(_knockOutH, &w, &h);
	Size ws = gm.WindowSize();
	DxLib::DrawGraph((ws.w - w) / 2, (ws.h - h) / 2, _knockOutH, true);
	if (--_kotimer == 0) {
		_updater = &BattleScene::FadeOutUpdate;
		_fadeTimer = 0;
	}
}

void
BattleScene::FadeOutUpdate(const KeyState& key) {
	GameMain& gm = GameMain::Instance();
	int brightness = (255 * (30-_fadeTimer)) / 30;

	DrawBase();

	DxLib::SetDrawBlendMode(DX_BLENDMODE_MUL, 0);
	DxLib::DrawBox(0, 0, gm.WindowSize().w, gm.WindowSize().h, GetColor(brightness, brightness, brightness), true);
	DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);


	if (++_fadeTimer == 30) {
		if (_playerwin == 2) {
			gm.ChangeScene(new BattleScene(_playerno,_enemyno,0, 0));
		}
		else if(_enemywin==2){
			gm.ChangeScene(new ContinueScene());
		}
		else {
			gm.ChangeScene(new BattleScene(_playerno, _enemyno, _playerwin, _enemywin));
		}
	}
}
void
BattleScene::Update(const KeyState& key) {
	(this->*_updater)(key);
}
