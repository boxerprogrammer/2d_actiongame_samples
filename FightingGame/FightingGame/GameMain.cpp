#include<DxLib.h>
#include "GameMain.h"

#include<stdio.h>
#include<DxLib.h>
#include"TitleScene.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;



struct SceneStruct;//�V�[���\���̑O���錾

//�֐��|�C���^�ptypedef�錾
typedef void(*SceneFunc)(SceneStruct* scene, const KeyState* key);

//�V�[���\����
struct SceneStruct {
	SceneFunc func;
	int wincount;
	int countdown;
	int timer;
};


///�^�C�g���V�[��
///@param scene �V�[�����
///@param key �L�[���
///@note �֐��|�C���^�Ŏ󂯓n������p�֐��ł�
void TitleSceneFunc(SceneStruct* scene, const KeyState* key);

///�L�����N�^�[�Z���N�g
///@param scene �V�[�����
///@param key �L�[���
///@note �֐��|�C���^�Ŏ󂯓n������p�֐��ł�
void CharacterselectSceneFunc(SceneStruct* scene, const KeyState* key);

///�o�g���V�[��
///@param scene �V�[�����
///@param key �L�[���
///@note �֐��|�C���^�Ŏ󂯓n������p�֐��ł�
void BattleScene(SceneStruct* scene, const KeyState* key);

///�R���e�B�j���[�V�[��
///@param scene �V�[�����
///@param key �L�[���
///@note �֐��|�C���^�Ŏ󂯓n������p�֐��ł�
void ContinueScene(SceneStruct* scene, const KeyState* key);

///�Q�[���I�[�o�[�V�[��
///@param scene �V�[�����
///@param key �L�[���
///@note �֐��|�C���^�Ŏ󂯓n������p�֐��ł�
void GameoverScene(SceneStruct* scene, const KeyState* key);

///�G���f�B���O
///@param scene �V�[�����
///@param key �L�[���
///@note �֐��|�C���^�Ŏ󂯓n������p�֐��ł�
void EndingSceneFunc(SceneStruct* scene, const KeyState* key);

///�G���f�B���O
///@param scene �V�[�����
///@param key �L�[���
///@note �֐��|�C���^�Ŏ󂯓n������p�֐��ł�
void StaffrollSceneFunc(SceneStruct* scene, const KeyState* key);

///��ʂ̒��S�ɕ�����`�悷��
///@param color �F
///@param str ����
void DrawCenterString(unsigned int color, const char* str) {
	DxLib::DrawString((SCREEN_WIDTH - GetDrawStringWidth(str, strlen(str))) / 2,
		(SCREEN_HEIGHT - GetFontSize()) / 2,
		str,
		color);
}




int _stageRyuRoof = 0;
int _stageRyuTemple = 0;
int _stageRyuSky = 0;









///�^�C�g���V�[��
void TitleSceneFunc(SceneStruct* scene, const KeyState* key) {
	if (key->state[KEY_INPUT_RETURN] && !key->lastState[KEY_INPUT_RETURN]) {
		scene->func = CharacterselectSceneFunc;
		scene->wincount = 0;
		scene->timer = 0;
	}
}

///�L�����N�^�[�Z���N�g
void CharacterselectSceneFunc(SceneStruct* scene, const KeyState* key) {
	
	if (key->state[KEY_INPUT_RETURN] && !key->lastState[KEY_INPUT_RETURN]) {
		scene->func = BattleScene;
	}

}

///�o�g���V�[��
void BattleScene(SceneStruct* scene, const KeyState* key) {
	char str[] = "�X�e�[�W 00";



	sprintf(str, "�X�e�[�W %02d", scene->wincount + 1);
	DrawCenterString(0xffffffff, str);
}

///�R���e�B�j���[�V�[��
void ContinueScene(SceneStruct* scene, const KeyState* key) {
	char str[] = "�R���e�B�j���[ 00";
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
		scene->timer = 300;//5�b�Ń^�C�g����
	}
	sprintf(str, "�R���e�B�j���[ %02d", scene->countdown);
	DrawCenterString(0xffffffff, str);
}

///�Q�[���I�[�o�[�V�[��
void GameoverScene(SceneStruct* scene, const KeyState* key) {
	if (--scene->timer == 0) {
		scene->func = TitleSceneFunc;
	}
	DrawCenterString(0xffffffff, "�Q�[���I�[�o�[");
}

///�G���f�B���O
void EndingSceneFunc(SceneStruct* scene, const KeyState* key) {
	if (--scene->timer == 0) {
		scene->func = StaffrollSceneFunc;
		scene->timer = 300;//5�b�Ŏ���
	}
	DrawCenterString(0xffffffff, "�G���f�B���O");
}

///�G���f�B���O
void StaffrollSceneFunc(SceneStruct* scene, const KeyState* key) {
	if (--scene->timer == 0) {
		scene->func = GameoverScene;
		scene->timer = 300;//5�b�Ŏ���
	}
	DrawCenterString(0xffffffff, "�X�^�b�t���[��");
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
	
	//������
	if (DxLib::DxLib_Init() == -1) {
		//�����񂩂������
		printf("DxLibInit Error");
		return;
	}
	DxLib::SetWindowText("0000000_�Z���Z�[");
	DxLib::SetDrawScreen(DX_SCREEN_BACK);
	SceneStruct scene = { TitleSceneFunc, 0, 0, 0 };
	KeyState key;

	DxLib::SetFontSize(36);
	LoadResources();

	ChangeScene(new TitleScene());
	
	//���C�����[�v
	while (ProcessMessage() == 0) {
		GetHitKeyStateAll(key.state);
		ClearDrawScreen();
		if (key.state[KEY_INPUT_ESCAPE]) {
			break;
		}
		
		//�֐��|�C���^�ł̏�ԑJ��
		scene.func(&scene, &key);

		//State�p�^�[���ł̏�ԑJ��
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

