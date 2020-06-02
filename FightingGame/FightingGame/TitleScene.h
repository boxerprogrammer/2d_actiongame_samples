
#pragma once
#include "Scene.h"
///タイトルシーン
class TitleScene :	public Scene
{
private:
	int _fadeTimer;//フェード用タイマー
	int _blinkTimer;//点滅タイマー
	int _waitTimer;//待ちタイマー
	int _titleH;//タイトルロゴハンドル
	int _pressStartH;//press start button画像ハンドル
	int _startSE;

	//１フレームごとに一度実行されるアップデート関数
	void (TitleScene::*_updater)(const KeyState& key);
	void FadeInUpdate(const KeyState& key);
	void NormalUpdate(const KeyState& key);
	void FastBlinkUpdate(const KeyState& key);
	void FadeOutUpdate(const KeyState& key);

public:
	TitleScene();
	~TitleScene();
	void Update(const KeyState& key);
};

