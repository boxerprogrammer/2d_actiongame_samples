#include <DxLib.h>
#include"application.h"
#include"scene/GameScene.h"
#include"Sound/BGMManager.h"

Application::Application(){
	Init();
}

void
Application::Init(){
	srand(DxLib::GetNowCount());
	DxLib::SetWindowText("スプラッター村");
	DxLib::ChangeWindowMode(TRUE);
	DxLib::SetGraphMode(800,600,32);
	DxLib_Init();
	DxLib::SetDrawScreen(DX_SCREEN_BACK);
	DxLib::SetFontSize(24);
	_scene.Reset(new SceneManager());
	DxLib::GetWindowSize(&_windowSize.width,&_windowSize.height);
	
}

void
Application::Start(){
	//アプリケーション終了通知まで周り続ける
	while(ProcessMessage()!=-1 && CheckHitKey(KEY_INPUT_ESCAPE)==0){
		DxLib::ClearDrawScreen();
		_scene->Update();
		DxLib::ScreenFlip();
		BGMManager::Instance().Update();;
	}
}

void 
Application::End(){
	DxLib_End();
}

Application::~Application(){
	End();
}

int 
Application::WindowHeight() const {
	return _windowSize.height;
}


int 
Application::WindowWidth() const{
	return _windowSize.width;
}

Application&
Application::Instance(){
	static Application instance;
	return instance;
}