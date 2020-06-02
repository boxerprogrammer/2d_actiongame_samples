#pragma once
#include "gamescene.h"
class AfterStage :
	public GameScene
{
public:
	AfterStage(void);
	~AfterStage(void);
	void Update(SceneManager& caller);
};

