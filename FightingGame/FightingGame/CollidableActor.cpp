#include "CollidableActor.h"
#include<DxLib.h>


CollidableActor::CollidableActor()
{
}


CollidableActor::~CollidableActor()
{
}

HitRect::HitRect(const HitRect& hr): rc(hr.rc), type(hr.type), parent(hr.parent), isDead(hr.isDead) {
}

HitRect::HitRect(Rect& irc,  RectType rt, CollidableActor* p) : 
	rc(irc),
	type(rt),
	parent(p),
	isDead(false){}

HitRect::HitRect(int x,int y ,int w, int h, RectType rt, CollidableActor* p) :
	rc(x,y,w,h),
	type(rt),
	parent(p),
	isDead(false) {}

void 
HitRect::Draw() {
	Position2 pos = parent->Position() + Vector2((parent->IsTurn() ? -rc.pos.x: rc.pos.x),rc.pos.y);
	switch (type) {
	case rt_attack:
		GetRect().Draw(0xff4040);
		break;
	case rt_damage:
		GetRect().Draw(0x40ff40);
		break;
	}
	
}

RectType 
HitRect::GetType()const {
	return type;
}

Rect
HitRect::GetRect()const {
	Rect retRec = rc;
	Position2 pos = parent->Position() + Vector2((parent->IsTurn() ? -rc.pos.x : rc.pos.x), rc.pos.y);
	retRec.pos = pos;
	return retRec;
}

void
HitRect::Kill() {
	isDead = true;
}

void
HitRect::Resurrect() {
	isDead = false;
}

bool 
HitRect::IsDead()const {
	return isDead;
}