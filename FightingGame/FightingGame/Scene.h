#pragma once

struct KeyState;

class Scene
{
public:
	Scene();
	virtual ~Scene();
	virtual void Update(const KeyState& state) = 0;
};

