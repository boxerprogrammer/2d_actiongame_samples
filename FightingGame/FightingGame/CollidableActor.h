#pragma once

#include"Geometry.h"
#include<vector>

///矩形種別
enum RectType {
	rt_none,//無効
	rt_damage,//やられ
	rt_attack//攻撃
};

class CollidableActor;

///切り抜き矩形用
struct CutGraphInfo {
	int handle;//ハンドル情報
	Rect rc;//切り抜き矩形情報
	CutGraphInfo() : handle (0),rc(){ }
	CutGraphInfo(int ihandle,Rect irc) : handle(0), rc(irc) { }
	CutGraphInfo(int ihandle, int x,int y,int w,int h) : handle(ihandle), rc(x,y,w,h) { }
};


///当たり判定用
class HitRect {
private:
	Rect rc;//矩形情報
	RectType type;//矩形種別(やられ、攻撃enum)
	CollidableActor* parent;//持ち主
	bool isDead;//死亡フラグ
public:
	//HitRect();
	HitRect(const HitRect& pachimon);
	///ヒット矩形
	///@param rc 矩形
	///@param ofst いらないかも
	///@param rt 矩形種別
	///@param p 持ち主のポインタ
	HitRect(Rect& rc,  RectType rt, CollidableActor* p);

	HitRect(int x, int y, int w, int h, RectType rt, CollidableActor* p);
	void SetParent(CollidableActor* p) { parent = p; }
	void Draw();
	RectType GetType()const;//どんな矩形やねん
	Rect GetRect()const;//矩形よこせやコラァ！
	bool IsDead()const;//死んどるけ？
	void Kill();//ぶっ殺す
	void Resurrect();//ぶっ生き返す
};

///衝突できる系のオブジェクトの基底クラス
class CollidableActor
{
protected:
	
public:
	CollidableActor();
	~CollidableActor();
	virtual const Position2& Position() const=0;
	virtual bool IsTurn()const = 0;
};

