#include "ClearScene.h"
#include"SceneManager.h"
#include"TitleScene.h"
#include"../Graphics/BaseDrawer.h"

ClearScene::ClearScene(void)
{
	_updater=&ClearScene::UpdateMain;
	_clearDrawer.Reset(new BaseDrawer("img/stage_clear.png"));
}


ClearScene::~ClearScene(void)
{
}


void
ClearScene::UpdateMain(SceneManager& caller){
	_clearDrawer->SetPosition(80,100);
	_clearDrawer->Draw();
	if(CheckNextRequest_Debug()){
		_updater=&ClearScene::UpdateBreaking;
	}
}

void 
ClearScene::UpdateBreaking(SceneManager& caller){
	caller.ChangeScene(new TitleScene());
}


void
ClearScene::Update(SceneManager& caller){
	(this->*_updater)(caller);
}