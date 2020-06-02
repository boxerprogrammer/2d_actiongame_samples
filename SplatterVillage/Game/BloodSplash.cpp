#include "BloodSplash.h"
#include"../DxLib/DxLib.h"
#include"Camera.h"
BloodSplash::BloodSplash(const Camera& camera,int handle,int x, int y,int w,int h,int num,bool turn,int interval):
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


BloodSplash::~BloodSplash(void)
{

}

void
BloodSplash::SetPosition(int x,int y){
	_x=x;
	_y=y;
}

void 
BloodSplash::Update(){
	if(_index>30){
		Kill();
	}
}


void
BloodSplash::Draw(){
	int offsetx=(_index%10)*_w;
	int offsety=(_index/10)*_h;
	DxLib::DrawRectGraph(_x-_camera.CurrentPos().x-(_turn?_w:0),
		_y-_camera.CurrentPos().y-214,
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
BloodSplash::Kill(){
	_suicide=true;
}
