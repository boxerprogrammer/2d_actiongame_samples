#pragma once
#include"Geometry.h"
#include "Scene.h"
#include<vector>
class ContinueScene :
	public Scene
{
private:
	int _timer;
	int _countdown;
	int _continueNumberSpriteH;
	std::vector<std::pair<Position2, Size>> _layouts;
public:
	ContinueScene();
	~ContinueScene();
	void Update(const KeyState& state);
};

