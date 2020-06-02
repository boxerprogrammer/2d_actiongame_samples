#pragma once

#include "../Geometry/geometry.h"

const float PI = 3.14159265;

inline float DegreeToRadian(float degree){
	return degree*2.f*PI/360.f;
}


inline float RadianToDigree(float radian){
	return radian*360.f/2.f/PI;
}




///���f���I�u�W�F�N�g
class Model3D{
private:
	bool _isAutoModel;//�����Ŕj��(�܂莩�E)����ׂ����f�����ǂ���(true�Ȃ�Ύ����Ŏ���)
	int _handle;//�RD���f���n���h��
	//@note �A�j���[�V�����ԍ��ƃA�^�b�`�ԍ��͕ʃ��m�Ȃ̂Œ��ӂ���悤��
	int _currentAnimNo;//���ݍĐ����̃A�j���[�V�����ԍ�
	int _attachedAnimNo;//���݂̃A�^�b�`�ԍ�
	float _animTime;//�A�j���[�V�����̌��ݒn�_(����t�˖ؖ�)
	float _duration;//�A�j���[�V�����̎�
	float _animSpeed;
	Vector3D _scale;//�g��k����
	Vector3D _pos;//��ʕ\���ʒu
	Vector3D _vel;//���f�����x
	bool _loop;//�A�j���[�V���������[�v���邩
	void Init(int handle);
public:
	Model3D(const char* path);
	Model3D(int handle);
	~Model3D();
	int Handle()const{
		return _handle;
	}
	
	///�A�j���[�V������ύX����(���O�w��Ver)
	///@param animname �ύX��A�j���[�V������
	void ChangeAnimation(const char* animname,bool loop=false);
	bool IsAnimEnd() const{
		return _animTime>_duration;
	}
	///�A�j���[�V������ύX����(�C���f�b�N�X�w��Ver)
	///@param animNo �ύX��A�j���[�V�����C���f�b�N�X
	void ChangeAnimation(const int animNo,bool loop=false);
	void SetVelocity(const Vector3D& vel){
		_vel = vel;
	}

	void ExecuteVelocity(){
		Move(_vel);
	}

	Vector3D Position()const;
	///�ʒu��ύX����
	void SetPosition(const Vector3D& pos);

	///���݈ʒu����ړ�����
	void Move(Vector3D& mov){
		_pos.x+=mov.x;
		_pos.y+=mov.y;
		_pos.z+=mov.z;
		SetPosition(_pos);
	}

	///���݂�XYZ��]���𓾂�
	Vector3D Rotation()const;

	///XYZ���ǂꂩ�ŉ�]
	///@param vec ���ꂼ��̎��łǂꂭ�炢��]�������������w�肷��
	///@attention �P�ʂ̓��W�A��
	void RotateXYZ(Vector3D& vec);

	///X���܂��̉�]
	///@attention �P�ʂ̓��W�A������
	void RotateX(float radian){
		RotateXYZ(Vector3D(radian,0.f,0.f));
	}

	///Y���܂��̉�]
	///@attention �P�ʂ̓��W�A������
	void RotateY(float radian){
		RotateXYZ(Vector3D(0.f,radian,0.f));
	}

	///Z���܂��̉�]
	///@attention �P�ʂ̓��W�A������
	void RotateZ(float radian){
		RotateXYZ(Vector3D(0.f,0.f,radian));
	}

	///�ϓ��g��k�����s��
	void SetScale(float scale);
	
	///���̃��f���Ɠ��������킹��
	///@param model �V���N�����������f��
	///@note ����̃��f����(�ϐg)�ɑΉ����邽��
	void Syncronize(const Model3D& model);

	///@usage ���t���[�����̊֐����R�[�����Ă��������B
	void Update(bool isDraw=true);

	void SetAnimSpeed(float speed){
		_animSpeed=speed;
	}
};