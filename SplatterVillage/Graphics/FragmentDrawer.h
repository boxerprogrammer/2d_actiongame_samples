#pragma once
#include "basedrawer.h"
///@todo 
#include <DxLib.h>
#include<vector>

struct FVector2D{
	float x;
	float y;
};

//破片
struct Fragment{
	DxLib::VERTEX* headVert;//三角形ポリゴン先頭頂点アドレス
	FVector2D center;//重心
	FVector2D velocity;//速度
	FVector2D accel;//加速度
	float angleVel;//角速度
	Fragment();
	void CalculateCenter();
	void Fall();
};

///画面をキャプチャして細かい断片にしてしまうドロワ
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
	//画面分割に必要な頂点
	std::vector<DxLib::VERTEX> _vertices;
	//破片情報
	std::vector<Fragment> _fragments;
	void CreateVertices(int divx,int divy,int sw,int sh);

};

