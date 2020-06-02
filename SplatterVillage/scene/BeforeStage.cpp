#include "BeforeStage.h"
#include"SceneManager.h"
#include"GamePlaying.h"
#include <DxLib.h>
#include"../Graphics/FragmentDrawer.h"
#include"../Sound/SoundEffect.h"
#include"../Sound/BGMManager.h"

BeforeStage::BeforeStage(void):_timer(120)
{
	_updater=&BeforeStage::UpdateMain;
	_drawer.Reset(new FragmentDrawer("img/stage_intro.png"));
	_drawer->SetLocation(loc_center);
	_icondrawer.Reset(new BaseDrawer("img/stage_icon.png"));
	_glassBreakSE.Reset(new SoundEffect("snd/osb/glass_break.wav"));
	BGMManager::Instance().Play(bid_before_stage);
}


BeforeStage::~BeforeStage(void)
{

}

void
BeforeStage::UpdateMain(SceneManager& caller){
	DxLib::DrawFormatString(100,100,0xffffffff,"Before Stage No=%d",caller.GetStageInfo().stageNo+1);
	_icondrawer->SetPosition(100,300);
	_icondrawer->Draw();
	if(CheckNextRequest_Debug()){
		_drawer->Capture();
		_drawer->Break();
		_glassBreakSE->Play();
		_updater=&BeforeStage::UpdateBreaking;
	}
}

void 
BeforeStage::UpdateBreaking(SceneManager& caller){
	if(--_timer==0){
		caller.ChangeScene(new GamePlaying());
	}
}

void
BeforeStage::Update(SceneManager& caller){
	

	//”j•Ð•`‰æ
	_drawer->Draw();
	(this->*_updater)(caller);
	if(CheckNextRequest_Debug()){

		caller.ChangeScene(new GamePlaying());
	}
}
