#pragma once
#include"../Geometry/geometry.h"
#include"../system/scoped_ptr.h"

class Model3D;
class Player;
class Camera;
//�G�A�ǂɃq�b�g�A�������͉�ʊO�ɏo���玩�����ł���
class Shot
{
friend class ShotFactory;
private:
	
	VectorInt2D _vec;
	ScopedPtr<Model3D> _model;
	const Camera* _camera;
	
protected:
	Rect _rect;
	Shot(const Camera*);
	Shot(const Camera*,int handle , Player& player);
	Shot* _prev;//���X�g��Ԃ̑O�V���b�g�ւ̎�Q��
	Shot* _next;//���X�g��Ԃ̑O�V���b�g�ւ̎�Q��
public:
	Shot* Prev(){return _prev;}
	Shot* Next(){return _next;}
	VectorInt2D Vector()const{return _vec;}
	virtual ~Shot(void);
	virtual void Update();
	virtual void OnDispose();
	virtual const Rect& HitRect() const;

	void DisplayDebugInfo();
};

//�����V���b�g
class NullShot : public Shot{
friend class ShotFactory;
private:
	NullShot();
public:
	~NullShot();
	virtual void Update();
	virtual void OnDispose();
};

///�e���H��
class ShotFactory{
private:
	int _handle;
	NullShot _topShot;
	ShotFactory();
	ShotFactory(const ShotFactory&);
	ShotFactory& operator=(const ShotFactory&);
public:
	static ShotFactory& Instance();
	Shot* Create(const Camera& camera,Player& player);
	Shot& TopShot();
	void DrawShots();
	void Clear();
};

