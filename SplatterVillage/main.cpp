#include <DxLib.h>
#include"application.h"
#include"resource.h"

int WINAPI WinMain(HINSTANCE,HINSTANCE,LPSTR,int){
	Application& app=Application::Instance();
	DxLib::SetWindowIconID(IDI_ICON1);

	//初期化処理


	//ゲームループ
	app.Start();
	return 0;
}
