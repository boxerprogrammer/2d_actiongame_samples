#include<DxLib.h>
#include "GameMain.h"

#include<stdio.h>
#include<DxLib.h>
#include"TitleScene.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;



struct SceneStruct;//シーン構造体前方宣言

//関数ポインタ用typedef宣言
typedef void(*SceneFunc)(SceneStruct* scene, const KeyState* key);

//シーン構造体
struct SceneStruct {
	SceneFunc func;
	int wincount;
	int countdown;
	int timer;
};


///タイトルシーン
///@param scene シーン情報
///@param key キー情報
///@note 関数ポインタで受け渡しする用関数です
void TitleSceneFunc(SceneStruct* scene, const KeyState* key);

///キャラクターセレクト
///@param scene シーン情報
///@param key キー情報
///@note 関数ポインタで受け渡しする用関数です
void CharacterselectSceneFunc(SceneStruct* scene, const KeyState* key);

///バトルシーン
///@param scene シーン情報
///@param key キー情報
///@note 関数ポインタで受け渡しする用関数です
void BattleScene(SceneStruct* scene, const KeyState* key);

///コンティニューシーン
///@param scene シーン情報
///@param key キー情報
///@note 関数ポインタで受け渡しする用関数です
void ContinueScene(SceneStruct* scene, const KeyState* key);

///ゲームオーバーシーン
///@param scene シーン情報
///@param key キー情報
///@note 関数ポインタで受け渡しする用関数です
void GameoverScene(SceneStruct* scene, const KeyState* key);

///エンディング
///@param scene シーン情報
///@param key キー情報
///@note 関数ポインタで受け渡しする用関数です
void EndingSceneFunc(SceneStruct* scene, const KeyState* key);

///エンディング
///@param scene シーン情報
///@param key キー情報
///@note 関数ポインタで受け渡しする用関数です
void StaffrollSceneFunc(SceneStruct* scene, const KeyState* key);

///画面の中心に文字を描画する
///@param color 色
///@param str 文字
void DrawCenterString(unsigned int color, const char* str) {
	DxLib::DrawString((SCREEN_WIDTH - GetDrawStringWidth(str, strlen(str))) / 2,
		(SCREEN_HEIGHT - GetFontSize()) / 2,
		str,
		color);
}




int _stageRyuRoof = 0;
int _stageRyuTemple = 0;
int _stageRyuSky = 0;









///タイトルシーン
void TitleSceneFunc(SceneStruct* scene, const KeyState* key) {
	if (key->state[KEY_INPUT_RETURN] && !key->lastState[KEY_INPUT_RETURN]) {
		scene->func = CharacterselectSceneFunc;
		scene->wincount = 0;
		scene->timer = 0;
	}
}

///キャラクターセレクト
void CharacterselectSceneFunc(SceneStruct* scene, const KeyState* key) {
	
	if (key->state[KEY_INPUT_RETURN] && !key->lastState[KEY_INPUT_RETURN]) {
		scene->func = BattleScene;
	}

}

///バトルシーン
void BattleScene(SceneStruct* scene, const KeyState* key) {
	char str[] = "ステージ 00";



	sprintf(str, "ステージ %02d", scene->wincount + 1);
	DrawCenterString(0xffffffff, str);
}

///コンティニューシーン
void ContinueScene(SceneStruct* scene, const KeyState* key) {
	char str[] = "コンティニュー 00";
	if (key->state[KEY_INPUT_RETURN] && !key->lastState[KEY_INPUT_RETURN]) {
		scene->func = CharacterselectSceneFunc;
		return;
	}
	else if (key->state[KEY_INPUT_SPACE] && !key->lastState[KEY_INPUT_SPACE]) {
		--scene->countdown;
		scene->timer = 60;
	}
	if (--scene->timer == 0) {
		--scene->countdown;
		scene->timer = 60;
	}

	if (scene->countdown < 0) {
		scene->func = GameoverScene;
		scene->timer = 300;//5秒でタイトルへ
	}
	sprintf(str, "コンティニュー %02d", scene->countdown);
	DrawCenterString(0xffffffff, str);
}

///ゲームオーバーシーン
void GameoverScene(SceneStruct* scene, const KeyState* key) {
	if (--scene->timer == 0) {
		scene->func = TitleSceneFunc;
	}
	DrawCenterString(0xffffffff, "ゲームオーバー");
}

///エンディング
void EndingSceneFunc(SceneStruct* scene, const KeyState* key) {
	if (--scene->timer == 0) {
		scene->func = StaffrollSceneFunc;
		scene->timer = 300;//5秒で次へ
	}
	DrawCenterString(0xffffffff, "エンディング");
}

///エンディング
void StaffrollSceneFunc(SceneStruct* scene, const KeyState* key) {
	if (--scene->timer == 0) {
		scene->func = GameoverScene;
		scene->timer = 300;//5秒で次へ
	}
	DrawCenterString(0xffffffff, "スタッフロール");
}

void LoadResources() {

	

}


GameMain::GameMain()
{
}


GameMain::~GameMain()
{
}

void
GameMain::ChangeScene(Scene* scene) {
	if (_scene != nullptr) {
		delete _scene;
	}
	_scene = scene;
}

void 
GameMain::Initialize() {

}
void 
GameMain::GameLoop() {

}
void 
GameMain::Run() {
	_windowSize.w= SCREEN_WIDTH;
	_windowSize.h=SCREEN_HEIGHT;

	DxLib::ChangeWindowMode(true);
	DxLib::SetWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	DxLib::SetGraphMode(SCREEN_WIDTH,SCREEN_HEIGHT,32);
	
	//初期化
	if (DxLib::DxLib_Init() == -1) {
		//あかんかったやで
		printf("DxLibInit Error");
		return;
	}
	DxLib::SetWindowText("0000000_センセー");
	DxLib::SetDrawScreen(DX_SCREEN_BACK);
	SceneStruct scene = { TitleSceneFunc, 0, 0, 0 };
	KeyState key;

	DxLib::SetFontSize(36);
	LoadResources();

	ChangeScene(new TitleScene());
	
	//メインループ
	while (ProcessMessage() == 0) {
		GetHitKeyStateAll(key.state);
		ClearDrawScreen();
		if (key.state[KEY_INPUT_ESCAPE]) {
			break;
		}
		
		//関数ポインタでの状態遷移
		scene.func(&scene, &key);

		//Stateパターンでの状態遷移
		_scene->Update(key);

		DxLib::ScreenFlip();
		memcpy(key.lastState, key.state, sizeof(key.state));
	}
	DxLib::DxLib_End();
}

int 
GameMain::WinCount()const {
	return _wincount;
}
void 
GameMain::IncrementWinCount(){
	++_wincount;
}

const Size& 
GameMain::WindowSize()const{
	return _windowSize;
}

