#pragma once
#include "Enemy.h"
#include"Missile.h"
#include<functional>
class Player;

///�X�^�[(�藠��)�����
class Star :public Enemy
{
private:
	enum AttackPosition {
		ap_upper,
		ap_middle,
		ap_lower
	};
	AttackPosition _attackposition;
	int _handle;//�摜�n���h��
	Player* _player;//�v���C���[�̎Q��
	std::function<void(void)> _subupdater;//�X�V���̊֐��I�u�W�F�N�g
	int _aiTimer;//AI�؂�ւ��p�^�C�}�[
	int _frame;//�A�j���[�V�����p�t���[��
	int _trembleX;//�k����X
	Position2 _pos;//�����̒��S���W
	std::vector<Missile> _missiles;//�e
	Rect _rc;//�����蔻���`
	int _hitStopTimer = 0;//�q�b�g�X�g�b�v�^�C�}�[
	int _endTimer;

	//��ԑJ�ڗp�֐��|�C���^
	typedef void (Star::*UpdateFunc_t)();
	UpdateFunc_t _updater;
	std::vector<UpdateFunc_t> _updatefuncTbl;

	std::function<void(void)> _drawer;

	//�U������t���O��Ԃ�
	bool IsTurn()const;

	//���̐U�镑�������߂�
	void DecideNextBevior();

	//�v���C���[�Ƃ̋�����Ԃ�
	unsigned int Distance();

	///�k���ɂ��X�I�t�Z�b�g��Ԃ�
	int TrembleX();

	//�O�i
	void StepIn();

	//���
	void BackStep();

	//�L�b�N
	void Kick();

	void PauseForEnd();
	void Dying();

	//�e
	void Shoot();
public:
	Star(int handle, Player* player);
	~Star();
	///���ݍ��W��Ԃ�
	const Position2& Position() const;

	///��ѓ���z���Ԃ�
	std::vector<Missile>& GetMissiles();
	
	///�G�̏�Ԃ��X�V���A�摜��`�悷��
	void Update();

	///�G�𕢂���`��Ԃ�
	Rect& GetRect();

	///��`�������������ɔ�������C�x���g(BattleScene����R�[�������)
	void OnCollided(RectType rt);

	void PauseForRendering() { 
		_endTimer = 60; 
		_updater = &Star::PauseForEnd;
	}

};

