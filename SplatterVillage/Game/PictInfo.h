#pragma once

struct PictInfo{
	int num;//アニメーションの際に何分割されているのか
	int handle;//画像のハンドル
	int interval;//インターバル
	PictInfo():num(1),handle(0),interval(6){}
	PictInfo(int inum,int ihandle,int ininterval=6):num(inum),handle(ihandle),interval(ininterval){}
};