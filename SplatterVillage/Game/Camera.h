#pragma once

#include"../Geometry/geometry.h"

class Player;

class Camera
{
private:
	PositionInt2D _position;
	const Player& _player;
	Size2D _range;
public:
	Camera(const Player& player);
	~Camera(void);
	
	///カメラが捕らえられる画面の範囲を返す
	void SetRange(const Size2D& range);

	///カメラの情報を更新
	void Update();

	///カメラの現在位置を返す
	const PositionInt2D& CurrentPos() const;

	///カメラから見たプレイヤーの位置を返す
	const PositionInt2D& PlayerPosition() const;

	const Size2D& ScreenRange()const;
};

