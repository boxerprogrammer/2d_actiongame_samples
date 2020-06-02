#pragma once

//ゲームシーンの基本クラス
class SceneManager;
class GameScene
{
public:
	GameScene(void)
	{
	}

	virtual ~GameScene(void)
	{
	}

	virtual void Update(SceneManager& caller)=0;

	///次シーンへの要求が行われたか
	///デバグ用関数
	///
	bool CheckNextRequest_Debug();
};

