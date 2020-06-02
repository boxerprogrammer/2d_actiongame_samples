#pragma once
#include "Enemy.h"
#include"Missile.h"
#include<functional>
class Player;

///スター(手裏剣)ちゃん
class Star :public Enemy
{
private:
	enum AttackPosition {
		ap_upper,
		ap_middle,
		ap_lower
	};
	AttackPosition _attackposition;
	int _handle;//画像ハンドル
	Player* _player;//プレイヤーの参照
	std::function<void(void)> _subupdater;//更新時の関数オブジェクト
	int _aiTimer;//AI切り替え用タイマー
	int _frame;//アニメーション用フレーム
	int _trembleX;//震えるX
	Position2 _pos;//自分の中心座標
	std::vector<Missile> _missiles;//弾
	Rect _rc;//当たり判定矩形
	int _hitStopTimer = 0;//ヒットストップタイマー
	int _endTimer;

	//状態遷移用関数ポインタ
	typedef void (Star::*UpdateFunc_t)();
	UpdateFunc_t _updater;
	std::vector<UpdateFunc_t> _updatefuncTbl;

	std::function<void(void)> _drawer;

	//振り向きフラグを返す
	bool IsTurn()const;

	//次の振る舞いを決める
	void DecideNextBevior();

	//プレイヤーとの距離を返す
	unsigned int Distance();

	///震えによるXオフセットを返す
	int TrembleX();

	//前進
	void StepIn();

	//後退
	void BackStep();

	//キック
	void Kick();

	void PauseForEnd();
	void Dying();

	//弾
	void Shoot();
public:
	Star(int handle, Player* player);
	~Star();
	///現在座標を返す
	const Position2& Position() const;

	///飛び道具配列を返す
	std::vector<Missile>& GetMissiles();
	
	///敵の状態を更新し、画像を描画する
	void Update();

	///敵を覆う矩形を返す
	Rect& GetRect();

	///矩形が当たった時に発生するイベント(BattleSceneからコールされる)
	void OnCollided(RectType rt);

	void PauseForRendering() { 
		_endTimer = 60; 
		_updater = &Star::PauseForEnd;
	}

};

