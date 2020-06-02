#include "StaffrollScene.h"
#include"GameMain.h"
#include"GameoverScene.h"
#include<DxLib.h>

StaffrollScene::StaffrollScene():_timer(300)
{
}


StaffrollScene::~StaffrollScene()
{
}


void
StaffrollScene::Update(const KeyState& key){
	DxLib::DrawString(100, 400, "Staffroll Scene", 0xffffffff);
	if (--_timer == 0) {
		GameMain::Instance().ChangeScene(new GameoverScene());
		return;
	}
}
