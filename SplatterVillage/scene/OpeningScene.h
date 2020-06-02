#pragma once
#include "gamescene.h"
#include "../system/shared_ptr.h"
#include "../system/scoped_ptr.h"
#include<vector>
class BaseDrawer;
class DrawFire;
class SoundEffect;
class TextDrawer;
class OpeningScene : public GameScene
{
	private:
		int _timer;
		int _thunderTimer;
		int _thunderInterval;
		int _thunderSound;
		ScopedPtr<SoundEffect> _thunderSE;
		std::vector<SharedPtr<DrawFire>> _drawFires;
		ScopedPtr<BaseDrawer> _drawer;
		ScopedPtr<BaseDrawer> _thunder1;
		ScopedPtr<BaseDrawer> _thunder2;
		ScopedPtr<TextDrawer> _textDrawer;
		std::vector<char> _script;
		void UpdateIntroducing(SceneManager& caller);
		void UpdateMain(SceneManager& caller);
		void UpdateFire(SceneManager& caller);
		void UpdateExiting(SceneManager& caller);

		void (OpeningScene::*_updater)(SceneManager& caller);
	public:
		OpeningScene(void);
		~OpeningScene(void);
		void Update(SceneManager& caller);
};

