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

///�A�v���P�[�V������ʃN���X
///���̃N���X�̐����A�j�������̂܂܃Q�[�����̂̎����ƂȂ�
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
	///�A�v���P�[�V�����N��
	void Start();
	///�A�v���P�[�V�����I���v��
	void RequestShutdown();

	int WindowWidth() const;
	int WindowHeight() const;

	static Application& Instance();
};
