#pragma once
#include<map>
#include<list>
#include<string>
#include"Character.h"
#include"../Geometry/geometry.h"
#include"../system/scoped_ptr.h"
#include"EffectBase.h"
#include"PictInfo.h"

class EffectFactory;
class Camera;

//���򖗃G�t�F�N�g
class BloodSplash : public EffectBase
{
	friend EffectFactory;
private:
	int _w;
	int _h;
	int _x;
	int _y;
	int _num;//���`�b�v��
	int _handle;//�摜�n���h��
	int _index;//�`�b�v�C���f�b�N�X
	
	bool _turn;
	const Camera& _camera;
public:
	BloodSplash(const Camera& camera,int handle,int x, int y,int w,int h,int num,bool turn,int interval=6);
	~BloodSplash(void);
	
	void SetPosition(int x,int y);

	//�X�V����
	void Update();

	//�`�揈��
	void Draw();

	void Kill();
};


