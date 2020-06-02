#include "GameObject.h"


GameObject::GameObject()
{
}


GameObject::~GameObject()
{
}

Ladder::Ladder(){
	_rc.size.w=32;
	_rc.size.h=32;
}

void
GameObject::InitPosition(PositionInt2D& pos){
	_rc.center.x = pos.x+_rc.size.w;
	_rc.center.y = pos.y+_rc.size.h;
}