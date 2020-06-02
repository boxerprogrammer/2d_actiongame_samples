#pragma once
#include"system/scoped_ptr.h"
#include"scene/SceneManager.h"
#include<list>

struct Size{
	int width;
	int height;
};

class GameScene;

struct TCB{
	char keystate[256];
};

///アプリケーション上位クラス
///このクラスの生成、破棄がそのままゲーム自体の寿命となる
class Application{
private:
	Application();
	Application(const Application& );
	Application& operator=(const Application& );
	ScopedPtr<GameScene> _currentScene;
	void Init();
	void End();
	TCB _tcb;
	ScopedPtr<SceneManager> _scene;
	Size _windowSize;
public:
	
	~Application();
	///アプリケーション起動
	void Start();
	///アプリケーション終了要求
	void RequestShutdown();

	int WindowWidth() const;
	int WindowHeight() const;

	static Application& Instance();
};
