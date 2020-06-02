#include "GameItem.h"
#include "Camera.h"
#include <DxLib.h>
#include "../Graphics/BaseDrawer.h"
GameItem::GameItem(const char* filename,Camera& camera,Rect& rect):_rect(rect),_camera(camera),_visible(false)
{
	_itemG.Reset(new BaseDrawer(filename));
}


GameItem::~GameItem(void)
{
}


void 
GameItem::SetPosition(const PositionInt2D& pos){
	_rect.center=pos;
}

void 
GameItem::Move(VectorInt2D& vec){
	_rect.center+=vec;
}

void 
GameItem::SetVisible(bool visible){
	_visible=visible;
}

const Rect& 
GameItem::GetRect()const{
	return _rect;
}

void 
GameItem::Draw(){
	if(!_visible){
		return;
	}
	_itemG->Draw(PositionInt2D(_rect.Left()-_camera.CurrentPos().x,_rect.Top()-_camera.CurrentPos().y));
}
