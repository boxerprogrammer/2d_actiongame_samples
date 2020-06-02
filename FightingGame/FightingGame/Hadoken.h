#pragma once
#include "CollidableActor.h"
class Hadoken :
	public CollidableActor
{
private:
	Position2 _pos;//���݂̍��W
	Vector2 _vel;//�ړ��x�N�g��
	HitRect _hitrect;//�������`
	CollidableActor& _parent;//���L�҂ւ̎Q��
	const CutGraphInfo _cutGraphInfo;//�؂蔲���摜���
	bool _isDead = false;
public:
	//Missile();
	Hadoken(const Hadoken&);
	Hadoken& operator=(const Hadoken&in);
	///�������ɏ��L�҂ւ̎Q�Ƃ͕K�{
	///@param cgi �؂蔲���摜���
	///@param x �������`X(���S����̃I�t�Z�b�g)
	///@param y �������`Y(���S����̃I�t�Z�b�g)
	///@param w �������`��
	///@param h �������`��
	///@param pos �������W
	///@param velocity ���x
	///@param p �e�ւ̎Q��
	Hadoken(const CutGraphInfo& cgi, int x, int y, int w, int h, const Position2& pos, const Vector2& velocity, CollidableActor& p);
	~Hadoken();
	const Position2& Position() const;
	bool IsTurn()const;
	void Update();
	HitRect& GetRect() { return _hitrect; }
	Hadoken& Activate();
	void SetPosition(int x, int y) {
		_pos.x = x;
		_pos.y = y;
	}
	void SetVelocity(int vx, int vy) {
		_vel.x = vx;
		_vel.y = vy;
	}
	bool IsDead() {
		return _isDead;
	}
	void Kill() {
		_isDead = true;
	}
	void Resurrect() {
		_isDead = false;
	}

};

