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

	///シーンを切り替える
	///@param scene 切り替えたいシーンオブジェクトへのポインタ
	///@usage ChangeScene(new TitleScene());
	///@remarks 切り替えのタイミングにて前のシーンは自動的に削除されます。
	void ChangeScene(GameScene* scene);
	StageInfo& GetStageInfo();
};

