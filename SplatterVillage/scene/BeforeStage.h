#pragma once
#include "gamescene.h"

#include "../system/scoped_ptr.h"

#include <vector>

class FragmentDrawer;
class BaseDrawer;
class SoundEffect;
class BeforeStage :
	public GameScene
{
private:
	ScopedPtr<FragmentDrawer> _drawer;
	ScopedPtr<BaseDrawer> _icondrawer;
	ScopedPtr<SoundEffect> _glassBreakSE;
	void (BeforeStage::*_updater)(SceneManager& caller);
	void UpdateMain(SceneManager& caller);
	void UpdateBreaking(SceneManager& caller);
	int _timer;
public:
	BeforeStage(void);
	~BeforeStage(void);
	void Update(SceneManager& caller);
};

