#include "HitEffect.h"
#include"../DxLib/DxLib.h"
#include"Camera.h"
HitEffect::HitEffect(const Camera& camera,int handle,int x, int y,int w,int h,int num,bool turn,int interval):
							_x(x),
							_y(y),
							_w(w),
							_h(h),
							_index(0),
							_handle(handle),
							_num(num),
							_turn(turn),
							_camera(camera),
							EffectBase()
{
}


HitEffect::~HitEffect(void)
{

}

void
HitEffect::SetPosition(int x,int y){
	_x=x;
	_y=y;
}

void 
HitEffect::Update(){
	if(_index>10){
		Kill();
	}
}


void
HitEffect::Draw(){
	int offsetx=(_index%5)*_w;
	int offsety=(_index/5)*_h;
	DxLib::DrawRectGraph(_x-_camera.CurrentPos().x-128,
		_y-_camera.CurrentPos().y-128,
		offsetx,
		offsety,
		_w,
		_h,
		_handle,
		true,
		_turn);
	++_index;
}

void 
HitEffect::Kill(){
	_suicide=true;
}
