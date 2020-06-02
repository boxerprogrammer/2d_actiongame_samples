#include "TileDrawer.h"
#include <DxLib.h>

TileDrawer::TileDrawer(void) : BaseDrawer()
{
}

TileDrawer::TileDrawer(const char* filename) : BaseDrawer(filename){
	DxLib::GetGraphSize( _handle,&_span.w,&_span.h);
	Size2D wsize;
	DxLib::GetWindowSize(&wsize.w,&wsize.h);
	repeatH=wsize.w/_span.w + (wsize.w % _span.w==0?0:1);

}

TileDrawer::~TileDrawer(void)
{
}


void
TileDrawer::Draw(){
	//ÉXÉNÉçÅ[ÉãÇÃÇΩÇﬂÇ…1ñáëΩÇ≠ï`âÊÇ∑ÇÈ
	for(int i=0;i<=repeatH;++i){
		DxLib::DrawGraph(i*_span.w-_scroll.x%_span.w + _position.x,
			_position.y,_handle,TRUE);
	}
}


void 
TileDrawer::SetScroll(PositionInt2D& scroll){
	_scroll=scroll;
}

void
TileDrawer::SetScroll(int x,int y){
	_scroll.x=x;
	_scroll.y=y;
}