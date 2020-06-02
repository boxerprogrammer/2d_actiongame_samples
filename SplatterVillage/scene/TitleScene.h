#pragma once
#include "gamescene.h"
#include "../system/scoped_ptr.h"
class Drawer;
class FragmentDrawer;
class LagDrawer;
class SoundEffect;
///ƒ^ƒCƒgƒ‹‰æ–Ê
class TitleScene : public GameScene
{
private:
	ScopedPtr<Drawer> _gamestart;
	ScopedPtr<Drawer> _drawer;
	ScopedPtr<Drawer> _drawerB;
	ScopedPtr<FragmentDrawer> _fragmentDrawer;
	ScopedPtr<LagDrawer> _dokuro;
	ScopedPtr<SoundEffect> _glassBreakSE;
	void(TitleScene::*_updater)(SceneManager& caller);
	void UpdatePre(SceneManager& caller);
	void UpdateMain(SceneManager& caller);
	void UpdateChanging(SceneManager& caller);
	void UpdateBreaking(SceneManager& caller);
	int _timer;
	int _capHandle;
public:
	TitleScene(void);
	~TitleScene(void);
	void Update(SceneManager& caller);
};

