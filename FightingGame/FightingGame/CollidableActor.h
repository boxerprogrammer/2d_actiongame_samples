#pragma once

#include"Geometry.h"
#include<vector>

///��`���
enum RectType {
	rt_none,//����
	rt_damage,//����
	rt_attack//�U��
};

class CollidableActor;

///�؂蔲����`�p
struct CutGraphInfo {
	int handle;//�n���h�����
	Rect rc;//�؂蔲����`���
	CutGraphInfo() : handle (0),rc(){ }
	CutGraphInfo(int ihandle,Rect irc) : handle(0), rc(irc) { }
	CutGraphInfo(int ihandle, int x,int y,int w,int h) : handle(ihandle), rc(x,y,w,h) { }
};


///�����蔻��p
class HitRect {
private:
	Rect rc;//��`���
	RectType type;//��`���(����A�U��enum)
	CollidableActor* parent;//������
	bool isDead;//���S�t���O
public:
	//HitRect();
	HitRect(const HitRect& pachimon);
	///�q�b�g��`
	///@param rc ��`
	///@param ofst ����Ȃ�����
	///@param rt ��`���
	///@param p ������̃|�C���^
	HitRect(Rect& rc,  RectType rt, CollidableActor* p);

	HitRect(int x, int y, int w, int h, RectType rt, CollidableActor* p);
	void SetParent(CollidableActor* p) { parent = p; }
	void Draw();
	RectType GetType()const;//�ǂ�ȋ�`��˂�
	Rect GetRect()const;//��`�悱����R���@�I
	bool IsDead()const;//����ǂ邯�H
	void Kill();//�Ԃ��E��
	void Resurrect();//�Ԃ������Ԃ�
};

///�Փ˂ł���n�̃I�u�W�F�N�g�̊��N���X
class CollidableActor
{
protected:
	
public:
	CollidableActor();
	~CollidableActor();
	virtual const Position2& Position() const=0;
	virtual bool IsTurn()const = 0;
};

