#include "Missile.h"
#include"GameMain.h"
#include<DxLib.h>
#include<iostream>

using namespace std;

Missile::Missile(const CutGraphInfo& cgi, int x, int y, int w, int h, const Position2& pos, const Vector2& velocity, CollidableActor& p):
	_cutGraphInfo(cgi),
	_hitrect(x,y,w,h,rt_attack,&p),
	_parent(p),
	_pos(pos),
	_vel(velocity),
	_angle(0.0f)
{
	
}


Missile::Missile(const Missile& in) :
	_cutGraphInfo(in._cutGraphInfo),
	_hitrect(in._hitrect),
	_parent(in._parent),
	_pos(in._pos),
	_vel(in._vel),
	_angle(0.0f){
	_hitrect.SetParent(this);
}


//Missile::Missile(const Missile&) {
//
//}
Missile::~Missile()
{
}


Missile& 
Missile::Activate() {
	_hitrect.SetParent(this);
	return *this;
}
const 
Position2& 
Missile::Position() const {
	return _pos;
}

bool Missile::IsTurn()const {
	return _parent.IsTurn();
}

void
Missile::Update() {
	_pos += _vel;
	if (IsTurn()) {
		_angle += 0.2f;
	}else{
		_angle -= 0.2f;
	}
	const Rect& rc= _cutGraphInfo.rc;
	DxLib::DrawRectRotaGraph2(_pos.x*2, _pos.y*2,
		rc.Left(), rc.Top(),
		rc.w, rc.h,
		rc.w / 2, rc.h / 2,
		2.0f, _angle,
		_cutGraphInfo.handle,
		true,
		IsTurn());
	const Size& size = GameMain::Instance().WindowSize();
	if(_pos.x+rc.w <0 || _pos.x-rc.w > size.w){
		Kill();
	}
}

Missile& 
Missile::operator=(const Missile& in) 
{ 
	_pos=in._pos;//現在の座標
	_vel=in._vel;//移動ベクトル
	_hitrect=in._hitrect;//当たり矩形
	_angle=in._angle;//角度保持用
	_isDead = in._isDead;//死亡フラグ
	this->_hitrect.SetParent(this);
	return *this; 
}