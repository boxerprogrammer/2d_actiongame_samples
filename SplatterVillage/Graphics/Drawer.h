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

	///�`��O�ɂ���Ă����������Ƃ�����΂����Ŏ���
	virtual void BeforeDraw()=0;

	///���t���[�����s�����`��֐�
	virtual void Draw()=0;

	///�`���ɂ���Ă����������Ƃ�����΂����Ŏ���
	virtual void AfterDraw()=0;

	virtual const PositionInt2D& Position()const{
		return _position;
	}

	///�ꏊ��ݒ肷��
	///@param x X���W
	///@param y Y���W
	virtual void SetPosition(int x,int y){
		_position.x=x;
		_position.y=y;
	}

	///�ړ����N����
	///@param vx X�ړ�����
	///@param vy Y�ړ�����
	virtual void MovePosition(int vx,int vy){
		_position.x+=vx;
		_position.y+=vy;
	}

	///�h�����I�u�W�F�N�g�̍폜
	void Delete(){
		_killRequest=true;
	}

	///�h�����I�u�W�F�N�g�������Ă���n���h����Ԃ�
	int Handle()const{
			return _handle;
		}
	
	///���ɍ폜���N�G�X�g���������Ă���̂��ǂ�����Ԃ��B
	bool RequestedKill() const{
		return _killRequest;
	}
};

