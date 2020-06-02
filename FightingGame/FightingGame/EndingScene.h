#pragma once
#include "Scene.h"
class EndingScene :
	public Scene
{
private:
	int _timer;
public:
	EndingScene();
	~EndingScene();
	void Update(const KeyState& key);
};

