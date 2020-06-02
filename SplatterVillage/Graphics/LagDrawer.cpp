#include "LagDrawer.h"
#include<DxLib.h>

LagDrawer::LagDrawer(const char* filepath,int intervalFrame,int lagNum):BaseDrawer(filepath),
	_intervalFrame(intervalFrame/lagNum),
	_lagNum(lagNum*2),
	_doShrink(false)
{
	_frame=1;
	_doShrink=true;
}

LagDrawer::~LagDrawer(void)
{
}

void
LagDrawer::Draw(){
	Positions_t::iterator it=_lagPositions.begin();
	int count=0;
	for(;it!=_lagPositions.end();++it){
		DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA,255 - (++count) * (256/_lagNum));
		//DxLib::DrawGraph(it->x,it->y,_handle,true);
		DxLib::DrawExtendGraph(it->x+count/2,it->y+count/2,
			it->x+_size.w-count/2,it->y+_size.h-count/2,_handle,true);
	}
	DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);
	BaseDrawer::Draw();
	if(_frame==0){
		if(_lagPositions.size()>=_lagNum){
			_lagPositions.pop_back();
			_lagPositions.pop_back();
		}
		if(!_lagPositions.empty()){
			VectorInt2D& p2 = _lagPositions.front();
			_lagPositions.push_front(_position);
			VectorInt2D vec = VectorInt2D(_position.x + (p2.x - _position.x) / 2,_position.y + (p2.y - _position.y) / 2);
			_lagPositions.push_front(vec);
		}else{
			_lagPositions.push_front(_position);
		}

	}
	_frame = (++_frame)%_intervalFrame;

	
}
