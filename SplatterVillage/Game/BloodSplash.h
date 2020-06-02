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

//血飛沫エフェクト
class BloodSplash : public EffectBase
{
	friend EffectFactory;
private:
	int _w;
	int _h;
	int _x;
	int _y;
	int _num;//総チップ数
	int _handle;//画像ハンドル
	int _index;//チップインデックス
	
	bool _turn;
	const Camera& _camera;
public:
	BloodSplash(const Camera& camera,int handle,int x, int y,int w,int h,int num,bool turn,int interval=6);
	~BloodSplash(void);
	
	void SetPosition(int x,int y);

	//更新処理
	void Update();

	//描画処理
	void Draw();

	void Kill();
};


