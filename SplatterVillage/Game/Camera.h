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
	
	///�J�������߂炦�����ʂ͈̔͂�Ԃ�
	void SetRange(const Size2D& range);

	///�J�����̏����X�V
	void Update();

	///�J�����̌��݈ʒu��Ԃ�
	const PositionInt2D& CurrentPos() const;

	///�J�������猩���v���C���[�̈ʒu��Ԃ�
	const PositionInt2D& PlayerPosition() const;

	const Size2D& ScreenRange()const;
};

