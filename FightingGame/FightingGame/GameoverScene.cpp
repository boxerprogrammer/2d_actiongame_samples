#include "GameoverScene.h"
#include"TitleScene.h"
#include"GameMain.h"
#include<DxLib.h>


GameoverScene::GameoverScene():_timer(300)
{
}


GameoverScene::~GameoverScene()
{
}

void
GameoverScene::Update(const KeyState& state) {
	DxLib::DrawString(100, 400, "GameOver Scene", 0xffffffff);
	if (--_timer==0) {
		GameMain::Instance().ChangeScene(new TitleScene());
		return;
	}
}
