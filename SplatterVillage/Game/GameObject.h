#pragma once

#include<map>
#include"../Geometry/geometry.h"
#include"../system/scoped_ptr.h"

class Player;

//とりあえずリアクションに関わるゲームオブジェクト
class GameObject
{
protected:
	Rect _rc;
public:
	GameObject();
	virtual ~GameObject();
	virtual void OnCollidied(Player& p)=0;
	Rect& Collider(){return _rc;};
	void InitPosition(PositionInt2D& pos);
	PositionInt2D Position(){return _rc.center;}
};

//ハシゴオブジェクト
class Ladder : public GameObject{
public:
	Ladder();
	void OnCollidied(Player& p){};
};