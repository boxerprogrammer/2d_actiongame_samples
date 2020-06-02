#pragma once
#include "Scene.h"
#include"Geometry.h"

class Player;
class Enemy;
class BattleScene :
	public Scene
{
private:
	Player* _player;
	Enemy* _enemy;
	int _kotimer;
	int _healthbarH;
	int _barH;
	int _bloodSplashH;
	int _bloodSplashFrame;
	bool _bloodTurn;
	int _hitEffectH;
	int _hitEffectFrame;
	Position2 _hitEffectPos;
	int _pixelArtH;
	int _fadeTimer;
	int _stageRyuSky;
	int _stageRyuRoof;
	int _stageRyuTemple;
	int _kungfuBattleBGM;
	int _knockOutH;
	int _knockOutVoiceH;
	int _vH;
	int _faceH;
	unsigned int _playerwin;
	unsigned int _enemywin;
	unsigned int _playerno;
	unsigned int _enemyno;
	void DrawBase();
	void DrawGauge();
	//フェードイン
	void FadeInUpdate(const KeyState& key);

	//バトル中
	void NormalUpdate(const KeyState& key);

	//KO演出状態
	void KnockOutUpdate(const KeyState& key);

	//フェードアウト状態
	void FadeOutUpdate(const KeyState& key);
	void(BattleScene::*_updater)(const KeyState& key);

public:
	BattleScene(unsigned int playerno,unsigned int enemyno , unsigned int playerwin = 0,unsigned int enemywin=0);
	~BattleScene();
	void Update(const KeyState& state);
};

