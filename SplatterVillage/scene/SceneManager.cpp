#include "SceneManager.h"
#include"TitleScene.h"
#include"OpeningScene.h"



SceneManager::SceneManager(void):_currentScene(new TitleScene())
{
	_stageInfo.stageNo=0;
	_stageInfo.maxStageNo=3;
}


SceneManager::~SceneManager(void)
{
}

void
SceneManager::Update(){
	_currentScene->Update(*this);
}

void 
SceneManager::ChangeScene(GameScene* newScene){
	_currentScene.Reset(newScene);
}

StageInfo& 
SceneManager::GetStageInfo(){
	return _stageInfo;
}