#pragma once
#include "gamescene.h"
#include "../system/scoped_ptr.h"

class BaseDrawer;
class Stage;



class GamePlaying : public GameScene
{
private:
	
	ScopedPtr<Stage> _stage;
	void InitPlaying(SceneManager& caller);
	void UpdatePlaying(SceneManager& caller);
	void StageClearAction(SceneManager& caller);
	void (GamePlaying::*_updater)(SceneManager& caller);
public:
	GamePlaying(void);
	~GamePlaying(void);
	void Update(SceneManager& caller);
};

