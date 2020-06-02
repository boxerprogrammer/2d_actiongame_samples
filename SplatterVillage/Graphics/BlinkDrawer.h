#pragma once
#include "basedrawer.h"
class BlinkDrawer :
	public BaseDrawer
{
private:
	int _cycle;
	int _blinkframe;
	bool _blinkable;
	bool _visible;
public:
	BlinkDrawer(void);
	BlinkDrawer(const char* filename,int cycle=60);
	void BlinkTime(unsigned int cycle);
	virtual ~BlinkDrawer(void);
	void Draw();
};

