#pragma once

#include"../Geometry/geometry.h"

enum GraphLocation{
	loc_upperleft,
	loc_lowerleft,
	loc_center,
	loc_upperright,
	loc_lowerrright,
};

class Drawer
{
protected:
	int _handle;
	VectorInt2D _position;
	bool _killRequest;
public:

	Drawer(void) : _handle(0),_killRequest(false),_position()
	{
	}

	virtual ~Drawer(void)
	{
	}

	virtual void SetLocation(GraphLocation loc)=0;

	///描画前にやっておきたいことがあればここで実装
	virtual void BeforeDraw()=0;

	///毎フレーム実行される描画関数
	virtual void Draw()=0;

	///描画後にやっておきたいことがあればここで実装
	virtual void AfterDraw()=0;

	virtual const PositionInt2D& Position()const{
		return _position;
	}

	///場所を設定する
	///@param x X座標
	///@param y Y座標
	virtual void SetPosition(int x,int y){
		_position.x=x;
		_position.y=y;
	}

	///移動を起こす
	///@param vx X移動距離
	///@param vy Y移動距離
	virtual void MovePosition(int vx,int vy){
		_position.x+=vx;
		_position.y+=vy;
	}

	///ドロワオブジェクトの削除
	void Delete(){
		_killRequest=true;
	}

	///ドロワオブジェクトが持っているハンドルを返す
	int Handle()const{
			return _handle;
		}
	
	///既に削除リクエストがかかっているのかどうかを返す。
	bool RequestedKill() const{
		return _killRequest;
	}
};

