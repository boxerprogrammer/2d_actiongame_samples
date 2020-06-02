#pragma once

#include"CollidableActor.h"
#include"Missile.h"
#include<vector>
#include<functional>

struct KeyState;
class Player;
class Enemy : public CollidableActor
{
protected:
	int _life;
private:
	int _handle;//�摜�n���h��
	Position2 _pos;//�G�̍��W
	Player* _player;//�v���C���[�̎Q��
	std::function<void(void)> _subupdater;//�X�V���̊֐��I�u�W�F�N�g
	int _aiTimer;//AI�؂�ւ��p�^�C�}�[
	int _frame;//�A�j���[�V�����p�t���[��
	int _trembleX;//�k����X
	
	Rect _rc;//�����蔻���`
	int _hitStopTimer = 0;//�q�b�g�X�g�b�v�^�C�}�[

	//�ȉ��͓�����ԑJ�ڂ̂��߂̊֐��ł��B
	//�����Ȃ�
	void Idle();

	//�߂Â�
	void StepIn();

	//�����
	void BackStep();

	//���̋�����ۂ�
	void Keep();

	//�T�C�R�N���b�V���[
	void DiveUp();

	//�T�C�R�N���b�V���[
	void DiveDown();

	//��ԑJ�ڗp�֐��|�C���^
	void (Enemy::*_updater)();

	//�U������t���O��Ԃ�
	bool IsTurn()const;

	//���̐U�镑�������߂�
	void DecideNextBevior();

	//�v���C���[�Ƃ̋�����Ԃ�
	unsigned int Distance();

	///�k���ɂ��X�I�t�Z�b�g��Ԃ�
	int TrembleX();
	std::vector<Missile> _missiles;
public:
	Enemy(int handle,Player* player);
	virtual ~Enemy();
	///�G�̏�Ԃ��X�V���A�摜��`�悷��
	virtual void Update();

	///�G�𕢂���`��Ԃ�
	virtual Rect& GetRect();

	///��`�������������ɔ�������C�x���g(BattleScene����R�[�������)
	virtual void OnCollided(RectType rt);

	///�G�̌��݈ʒu��Ԃ�
	virtual const Position2& Position()const;

	virtual std::vector<Missile>& GetMissiles();

	virtual int GetLife()const { return _life; }
	virtual void PauseForRendering() {}


};

