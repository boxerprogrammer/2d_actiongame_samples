#pragma once
#include"Character.h"
#include"../Geometry/geometry.h"
#include"../system/scoped_ptr.h"
class SoundEffect;
class Camera;
class Model3D;
class Ladder;

enum PlayerState{
	pst_armor,
	pst_naked,
	pst_dying,//���ɒ�
	pst_dead//��
};

class Player{

private:
	ScopedPtr<SoundEffect> _deadSE;//�f����SE
	ScopedPtr<SoundEffect> _throwSE;//�Ȃ�SE
	ScopedPtr<SoundEffect> _jumpSE;//�W�����vSE
	PlayerState _pst;
	PositionInt2D _position;
	PositionInt2D _lastPosition;
	ScopedPtr<Model3D> _model;//�W���̊Z�����f��
	ScopedPtr<Model3D> _modelN;//naked��N�B�v�͊Z�������ꂽ�ۂ̃��f���ł��ˁB
	ScopedPtr<Model3D> _modelD;//dead��D�B�v�͎��̃��f���ł��ˁB
	Model3D* _currentModel;//���f���ւ̎�Q��(���̃|�C���^��������Ă͂Ȃ�Ȃ��I�I)
	Rect _rect;
	VectorInt2D _velocity;
	int _crashTimer;//�_���[�W�G�t�F�N�g����
	int _ultimateTimer;//���G����

	bool _isPressShotButton;

	//�v���C���[�X�e�[�^�X
	bool _isGround;//�n�ʂ��H
	bool _isMoved;//���쒆���H
	bool _isPrejumping;//�W�����v�\�����쒆���H
	bool _isJumping;//�W�����v�����H
	bool _isLanding;//���n�����H
	bool _isThrowing;//���퓊�������H
	bool _isCrouch;//���Ⴊ�ݒ����H
	bool _isClear;//�N���A���쒆���H

	void OnDamaged();
	void OnDead();
	void OnPrejump();
	void OnJump();
	void OnStop();
	void OnLanding();
	void OnLaddering();//��q�̂ڂ�C�x���g
	void CrashEffect(const Camera& camera);

	void DisplayDebugInfo(const Camera& camera);
	
public:

	Player();
	~Player();
	const PositionInt2D& CurrentPos() const;
	const VectorInt2D& Velocity() const;
	CharacterDirection Direction()const;
	bool IsMoved() const{
		return _isMoved;
	}

	bool IsStanding()const;
	bool IsCrouching()const{return _isCrouch;}
	bool IsNotJumping()const;
	void Draw(const Camera& camera);

	const PlayerState State() const;
	//���n�C�x���g
	void OnGround();
	void OnAir();
	
	//�G�q�b�g�C�x���g
	void OnHitEnemy();
	void OnClear();
	const Rect& HitRect() const;
	const int LastFootPosY() const;
	void KillMe();
	void Adjust(const Size2D& size,CharacterDirection dir=dir_none);

	//�n�V�S�t���O
	bool _isOverlappedLadder=false;
	bool _isLaddering=false;
	//�n�V�S�C�x���g
	void OnHitLadder(Ladder& ladder);
	void OnNotHitLadder();
	bool IsLaddering();
	bool IsOverlappedLadder();

};



