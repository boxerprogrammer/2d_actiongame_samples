#pragma once
#include "CollidableActor.h"
class Hadoken :
	public CollidableActor
{
private:
	Position2 _pos;//現在の座標
	Vector2 _vel;//移動ベクトル
	HitRect _hitrect;//当たり矩形
	CollidableActor& _parent;//所有者への参照
	const CutGraphInfo _cutGraphInfo;//切り抜き画像情報
	bool _isDead = false;
public:
	//Missile();
	Hadoken(const Hadoken&);
	Hadoken& operator=(const Hadoken&in);
	///生成時に所有者への参照は必須
	///@param cgi 切り抜き画像情報
	///@param x 当たり矩形X(中心からのオフセット)
	///@param y 当たり矩形Y(中心からのオフセット)
	///@param w 当たり矩形幅
	///@param h 当たり矩形高
	///@param pos 発生座標
	///@param velocity 速度
	///@param p 親への参照
	Hadoken(const CutGraphInfo& cgi, int x, int y, int w, int h, const Position2& pos, const Vector2& velocity, CollidableActor& p);
	~Hadoken();
	const Position2& Position() const;
	bool IsTurn()const;
	void Update();
	HitRect& GetRect() { return _hitrect; }
	Hadoken& Activate();
	void SetPosition(int x, int y) {
		_pos.x = x;
		_pos.y = y;
	}
	void SetVelocity(int vx, int vy) {
		_vel.x = vx;
		_vel.y = vy;
	}
	bool IsDead() {
		return _isDead;
	}
	void Kill() {
		_isDead = true;
	}
	void Resurrect() {
		_isDead = false;
	}

};

