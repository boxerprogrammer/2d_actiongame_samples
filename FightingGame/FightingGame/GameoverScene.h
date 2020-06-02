#pragma once
#include "Scene.h"

class GameoverScene :
	public Scene
{
private:
	int _timer;
public:
	GameoverScene();
	~GameoverScene();
	void Update(const KeyState& state);
};




