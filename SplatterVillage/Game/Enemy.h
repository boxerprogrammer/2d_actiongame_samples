#pragma once

#include<map>
#include<string>
#include"Character.h"
#include"../Geometry/geometry.h"
#include"../system/scoped_ptr.h"

#include"BloodSplash.h"

#include"PictInfo.h"


class EnemyFactory;
class Camera;
class Model3D;
class Player;
class Shot;



///�G�L�������N���X
class Enemy{
protected:
	int _crashTimer;
	bool _isDead;
	PositionInt2D _initialPos;
	PositionInt2D _pos;
	Vector2D<int> _vel;
	Rect _rect;
	int _life;
	const Camera& _camera;
	const EnemyFactory& _factory;
	void CrashEffect();
public:
	Enemy(const Camera& camera,const PositionInt2D& position,const EnemyFactory& factory);
	virtual ~Enemy();

	void SetLife(int life){
		_life=life;
	}

	const Rect& HitRect()const{
		return _rect;
	}

	bool IsDead()const{return _isDead;}

	
	virtual void OnDamage(Shot*);
	virtual void OnDead(Shot*);
	virtual void OnWakeUp(){}

	///�ʏ�ȊO�̒ǉ��A�N�V�����̃Z�b�g
	virtual void SetAppearAction(int handle,int num,int interval=6){}
	virtual void SetDisppearAction(int handle){}
	virtual void SetDeathAction(int handle){}

	virtual void SetPosition(const PositionInt2D& );

	///���݈ʒu�̏�����
	virtual void InitPosition(const PositionInt2D& );
	
	virtual void Disappear(){}

	///�h��
	virtual void Resurrect(){}

	///
	virtual const PositionInt2D& Position()const{
		return _pos;
	}
		
	///�X�V
	virtual void Update()=0;

	///�`��
	virtual void Draw()=0;
};



///2D�G�̐���A�\�����s����
class Enemy2D : public Enemy
{
	friend EnemyFactory;
	protected:
		
		Enemy2D(const Camera& camera,int handle,const EnemyFactory&,bool animTurn=false);
		Enemy2D(const Camera& camera,int handle,int x,int y,const EnemyFactory&,bool animTurn=false);
		///�A�j���[�V��������̏ꍇ�̃X�^���_�[�h
		///@param camera �J�����ւ̎Q��
		///@param handle �摜�n���h��
		///@param w �A�j���[�V�����P��������̕�
		///@param h �A�j���[�V�����P��������̍���
		///@param interval �A�j���[�V�����P���̕\������(�f�t�H���g6�Ȃ͓̂K��)
		Enemy2D(const Camera& camera,int handle,int w,int h,int num,const EnemyFactory& f,int interval=6,bool animTurn=false);
		int _w,_h;
		std::map<int,PictInfo> _handles;
		int _key;
		bool _isAnim;
		int _interval;
		int _num;
		int _idx;
		int _waitTimer;
		BloodSplash* _blood;//�����Ԃ��G�t�F�N�g�ւ̎�Q��
		HitEffect* _hiteffect;//�q�b�g�G�t�F�N�g�ւ̎�Q��
		bool _turn;
		const bool _animTurn;//�A�j���[�V�����̃^�[���Đ�
	public:
		
		virtual ~Enemy2D(void);
		
		///�X�V
		virtual void Update();

		///�`��
		virtual void Draw();

		virtual void OnDead(Shot*);
};

///3D�G�\���N���X
class Enemy3D : public Enemy
{
	friend EnemyFactory;
	private:
		Enemy3D(const Camera& camera,int handle,const EnemyFactory& f);
		Enemy3D(const Camera& camera,int handle,int x,int y,const EnemyFactory& f);
		
		ScopedPtr<Model3D> _model;
		
		void UpdateAppear();
		void UpdateNeutral();
		void (Enemy3D::*_updater)();
	public:
		
		virtual ~Enemy3D(void);
		
		//�ꏊ�̕ύX
		virtual void SetPosition(const PositionInt2D& );

		///�X�V
		virtual void Update();

		///�`��
		virtual void Draw();
};


///�G���
enum EnemyType{
	enm_enemy,
	enm_dokuro,//�h�N��
	enm_crow,//�G
	enm_eyeball,//�ڋ�
	enm_lipman//�N�`�r���Q
};

///�GSE���
enum EnemySeType{
	none,
	se_dokuro_appear,
};

///�G�L�����𐶂ݏo���t�@�N�g���N���X
///�t�@�N�g�������ǃV���O���g���N���X�ł͂Ȃ�
///���g�I�ɂ́A�O���t�B�N�X�n���h���ɂ��Ă�Flyweight�ɂȂ��Ă���B
class EnemyFactory{
	public:
		EnemyFactory(const Camera&,const Player& player);
		~EnemyFactory();

		///�t�@�C�������󂯎��A��������G�I�u�W�F�N�g��Ԃ�
		///@note �����t�@�C�����w�肳�ꂽ�ꍇ�́A���̖��O�����Ƀ��[�h�ς݂̉摜���g��
		Enemy* Create(const char* name ,EnemyType type=enm_enemy);

		///�������̃A�N�V�����Ɋ֘A����t�@�C����o�^����
		///@param enemy �o�^�������G�I�u�W�F�N�g
		///@param filename �t�@�C����
		void SetAppearAction(Enemy* enemy,const char* filename);

		///���Ŏ��̃A�N�V�����Ɋ֘A����t�@�C����o�^����
		///@param enemy �o�^�������G�I�u�W�F�N�g
		///@param filename �t�@�C����
		void SetDisappearAction(Enemy* enemy,const char* filename);

		///���S���̃A�N�V�����Ɋ֘A����t�@�C����o�^����
		///@param enemy �o�^�������G�I�u�W�F�N�g
		///@param filename �t�@�C����
		void SetDeathAction(Enemy* enemy,const char* filename);

		void PlaySE(EnemySeType type) const;

		void Destroy();
	private: 
		//�R�s�[�E����֎~
		EnemyFactory(const EnemyFactory&);
		EnemyFactory& operator=(const Enemy&);
		typedef std::map<std::string,int> namemap_t;
		namemap_t _handles;
		std::map<EnemySeType,int> _seMap;
		const Camera& _camera;
		const Player& _player;
		
};

