#pragma once
#include "Scene.h"
class StaffrollScene :
	public Scene
{
private:
	int _timer;
public:
	StaffrollScene();
	~StaffrollScene();
	void Update(const KeyState& key);
};

