#include "BaseDrawer.h"
#include <DxLib.h>
#include "../application.h"
BaseDrawer::BaseDrawer(void) : Drawer() , _adjustWindow(false)
{
	
}

BaseDrawer::BaseDrawer(const char* filepath) : Drawer() , _adjustWindow(false){
	Load(filepath);
}

BaseDrawer::~BaseDrawer(void)
{
	DeleteGraph(_handle);
}

void
BaseDrawer::AdjustWindow(){
	_adjustWindow=true;
}

void
BaseDrawer::Load(const char* filepath){
	_handle = DxLib::LoadGraph(filepath);
	GetGraphSize(_handle,&_size.w,&_size.h);
}

void
BaseDrawer::Draw(){
	if(_adjustWindow){
		const Application& application = Application::Instance();
		DxLib::DrawExtendGraph(0,0,application.WindowWidth(),
			application.WindowHeight(),_handle,TRUE);
	}else{
		DxLib::DrawGraph(_position.x,_position.y,_handle,TRUE);
	}
}

void 
BaseDrawer::Draw(int alpha){
	if(_adjustWindow){
		const Application& application = Application::Instance();
		DxLib::DrawExtendGraph(0,0,application.WindowWidth(),
			application.WindowHeight(),_handle,TRUE);
	}else{
		DxLib::DrawGraph(_position.x,_position.y,_handle,TRUE);
	}
}

void 
BaseDrawer::Draw(PositionInt2D& pos){
	DxLib::DrawGraph(pos.x,pos.y,_handle,TRUE);
}

void 
BaseDrawer::Draw(PositionInt2D& pos,int alpha){
	DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA,alpha);
	DxLib::DrawGraph(pos.x,pos.y,_handle,TRUE);
	DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);
}

void
BaseDrawer::SetLocation(GraphLocation location){
	if(location==loc_center){
		int sw,sh;
		DxLib::GetWindowSize(&sw,&sh);
		int gw,gh;
		DxLib::GetGraphSize(_handle,&gw,&gh);
		_position.x = (sw-gw)/2;
		_position.y = (sh-gh)/2;
	}
}