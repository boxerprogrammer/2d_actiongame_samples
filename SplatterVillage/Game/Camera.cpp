#include "Camera.h"
#include "Player.h"

Camera::Camera(const Player& player) : _player(player),_position(0,280),_range()
{
}


Camera::~Camera(void)
{
}

const PositionInt2D&
Camera::CurrentPos() const{
	return _position;
}
const PositionInt2D& 
Camera::PlayerPosition() const{
	return _player.CurrentPos()-_position;
}

const Size2D&
Camera::ScreenRange()const{
	return Size2D(800,600);
}

void
Camera::Update(){
	//��ʒ������i��ł���̂Ȃ�΃J�����𓮂����B
	//���Ă͂����Ȃ��͈͂������Ă��܂��O�ɃJ�������~�߂�
	if( _player.CurrentPos().x > 336 && _player.CurrentPos().x < _range.w-496){
		_position.x=_player.CurrentPos().x-336;
	}
}

void
Camera::SetRange(const Size2D& range){
	_range=range;
}

