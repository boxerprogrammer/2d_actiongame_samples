#include "GamePlaying.h"
#include <DxLib.h>
#include "SceneManager.h"
#include "BeforeStage.h"
#include "AfterStage.h"
#include "ClearScene.h"
#include "TitleScene.h"
#include "../Game/Stage.h"
#include "../Game/Player.h"
#include "../Game/BloodSplash.h"
#include "../Sound/BGMManager.h"

GamePlaying::GamePlaying(void)
{
	_stage.Reset(new Stage());
	_updater = &GamePlaying::InitPlaying;
}


GamePlaying::~GamePlaying(void)
{
}

void
GamePlaying::InitPlaying(SceneManager& caller){
	BGMManager::Instance().Play(BGMID::bid_stage_1);
	_updater=&GamePlaying::UpdatePlaying;
}

void
GamePlaying::UpdatePlaying(SceneManager& caller){
	_stage->Update();//OCの際はOCUpdateを使用すること
	EffectFactory::Instance().Update();
	EffectFactory::Instance().Draw();
	if(_stage->GetPlayer().State()==pst_dead){
		caller.ChangeScene(new BeforeStage());
	}else if(_stage->IsClear()){
		_updater=&GamePlaying::StageClearAction;
	}
}

void 
GamePlaying::StageClearAction(SceneManager& caller){
	_stage->Update();//OCの際はOCUpdateを使用すること
	_stage->StageClearAction();
	if(_stage->EndOfClearAction()){
		caller.ChangeScene(new ClearScene());
	}
}

void
GamePlaying::Update(SceneManager& caller){
	(this->*_updater)(caller);
}
