#include "EndingScene.h"
#include<DxLib.h>
#include"GameMain.h"
#include"StaffrollScene.h"

EndingScene::EndingScene():_timer(300)
{

}


EndingScene::~EndingScene()
{
}

void 
EndingScene::Update(const KeyState& key){
	DxLib::DrawString(100, 200, "Ending Scene", 0xffffffff);
	if(--_timer==0){
		GameMain::Instance().ChangeScene(new StaffrollScene());
	}
}
