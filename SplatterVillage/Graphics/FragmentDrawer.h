#pragma once
#include "basedrawer.h"
///@todo 
#include <DxLib.h>
#include<vector>

struct FVector2D{
	float x;
	float y;
};

//�j��
struct Fragment{
	DxLib::VERTEX* headVert;//�O�p�`�|���S���擪���_�A�h���X
	FVector2D center;//�d�S
	FVector2D velocity;//���x
	FVector2D accel;//�����x
	float angleVel;//�p���x
	Fragment();
	void CalculateCenter();
	void Fall();
};

///��ʂ��L���v�`�����čׂ����f�Ђɂ��Ă��܂��h����
class FragmentDrawer :
	public BaseDrawer
{
public:
	FragmentDrawer(const char* fileName);
	FragmentDrawer(void);
	~FragmentDrawer(void);

	///@see Drawer
	void Draw();
	void Break();
	void Capture();
private:
	int _capHandle;
	bool _breaking;
	//��ʕ����ɕK�v�Ȓ��_
	std::vector<DxLib::VERTEX> _vertices;
	//�j�Џ��
	std::vector<Fragment> _fragments;
	void CreateVertices(int divx,int divy,int sw,int sh);

};

