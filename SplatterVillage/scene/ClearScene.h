#pragma once

#include "gamescene.h"
#include "../system/scoped_ptr.h"

class BaseDrawer;


class ClearScene : public GameScene
{
private:
	ScopedPtr<BaseDrawer> _clearDrawer;
	void (ClearScene::*_updater)(SceneManager& caller);
	void UpdateMain(SceneManager& caller);
	void UpdateBreaking(SceneManager& caller);

public:
	ClearScene(void);
	~ClearScene(void);
	void Update(SceneManager& caller);
};

