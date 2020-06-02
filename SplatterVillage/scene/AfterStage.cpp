#include "AfterStage.h"
#include "BeforeStage.h"
#include "SceneManager.h"
#include <DxLib.h>
#include "TitleScene.h"
AfterStage::AfterStage(void)
{
}


AfterStage::~AfterStage(void)
{
}

void 
AfterStage::Update(SceneManager& caller){
	DxLib::DrawFormatString(100,100,0xffffffff,"After Stage No=%d",caller.GetStageInfo().stageNo+1);
	if(CheckNextRequest_Debug()){
		++caller.GetStageInfo().stageNo;
		if(caller.GetStageInfo().stageNo<caller.GetStageInfo().maxStageNo){
			caller.ChangeScene(new BeforeStage());
		}else{
			//ゲームクリア画面
			//今はタイトルへ
			caller.ChangeScene(new TitleScene());
		}
	}
}