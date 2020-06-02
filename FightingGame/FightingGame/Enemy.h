#pragma once

#include"CollidableActor.h"
#include"Missile.h"
#include<vector>
#include<functional>

struct KeyState;
class Player;
class Enemy : public CollidableActor
{
protected:
	int _life;
private:
	int _handle;//画像ハンドル
	Position2 _pos;//敵の座標
	Player* _player;//プレイヤーの参照
	std::function<void(void)> _subupdater;//更新時の関数オブジェクト
	int _aiTimer;//AI切り替え用タイマー
	int _frame;//アニメーション用フレーム
	int _trembleX;//震えるX
	
	Rect _rc;//当たり判定矩形
	int _hitStopTimer = 0;//ヒットストップタイマー

	//以下は内部状態遷移のための関数です。
	//動かない
	void Idle();

	//近づく
	void StepIn();

	//離れる
	void BackStep();

	//一定の距離を保つ
	void Keep();

	//サイコクラッシャー
	void DiveUp();

	//サイコクラッシャー
	void DiveDown();

	//状態遷移用関数ポインタ
	void (Enemy::*_updater)();

	//振り向きフラグを返す
	bool IsTurn()const;

	//次の振る舞いを決める
	void DecideNextBevior();

	//プレイヤーとの距離を返す
	unsigned int Distance();

	///震えによるXオフセットを返す
	int TrembleX();
	std::vector<Missile> _missiles;
public:
	Enemy(int handle,Player* player);
	virtual ~Enemy();
	///敵の状態を更新し、画像を描画する
	virtual void Update();

	///敵を覆う矩形を返す
	virtual Rect& GetRect();

	///矩形が当たった時に発生するイベント(BattleSceneからコールされる)
	virtual void OnCollided(RectType rt);

	///敵の現在位置を返す
	virtual const Position2& Position()const;

	virtual std::vector<Missile>& GetMissiles();

	virtual int GetLife()const { return _life; }
	virtual void PauseForRendering() {}


};

