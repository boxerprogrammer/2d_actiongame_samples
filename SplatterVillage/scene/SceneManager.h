#pragma once

#include"../system/scoped_ptr.h"

class GameScene;

struct StageInfo{
	int stageNo;
	int maxStageNo;
};

class SceneManager
{
private:
	ScopedPtr<GameScene> _currentScene;
	StageInfo _stageInfo;
public:
	SceneManager(void);
	~SceneManager(void);
	void Update();

	///�V�[����؂�ւ���
	///@param scene �؂�ւ������V�[���I�u�W�F�N�g�ւ̃|�C���^
	///@usage ChangeScene(new TitleScene());
	///@remarks �؂�ւ��̃^�C�~���O�ɂđO�̃V�[���͎����I�ɍ폜����܂��B
	void ChangeScene(GameScene* scene);
	StageInfo& GetStageInfo();
};

