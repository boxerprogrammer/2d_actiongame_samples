#pragma once

#include"Drawer.h"
#include<list>

class Visitor{
public:
	Visitor(){};
	~Visitor(){};
};

class GraphicsUpdater
{
private:
	std::list<Drawer*> _drawers;
public:
	GraphicsUpdater(void);
	~GraphicsUpdater(void);
	void Update();
};

