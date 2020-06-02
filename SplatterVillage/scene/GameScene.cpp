#include"GameScene.h"
#include <DxLib.h>
bool _lastNextRequest_Debug=false;
bool
GameScene::CheckNextRequest_Debug()
{
	bool retval=false;

	if(DxLib::CheckHitKey(KEY_INPUT_Z)){
		if( !_lastNextRequest_Debug ){
			retval=true;
		}
		_lastNextRequest_Debug=true;
	}else{
		_lastNextRequest_Debug=false;
	}

	return retval;;
}