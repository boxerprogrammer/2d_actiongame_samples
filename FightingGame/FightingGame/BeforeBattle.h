#pragma once
#include "Scene.h"
class BeforeBattle :
	public Scene
{
	int _beforeBattleH;
	int _bgmH;
	int _doraH;
	int _pixelArtH;
	int _timer;
	int _fadeTimer;
	int _playerno;
	int _enemyno;
	void FadeInUpdate(const KeyState& key);
	void NormalUpdate(const KeyState& key);
	void FadeOutUpdate(const KeyState& key);
	void(BeforeBattle::*_updater)(const KeyState& key);
public:
	BeforeBattle(int playerno=0,int enemyno=0);
	~BeforeBattle();
	void Update(const KeyState& key);
};

