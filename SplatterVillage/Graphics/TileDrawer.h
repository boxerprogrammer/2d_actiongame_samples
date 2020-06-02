#pragma once

#include"BaseDrawer.h"
 

class TileDrawer : public BaseDrawer
{
private:
	Size2D _span;
	int repeatH;
	int repeatV;
	PositionInt2D _scroll;
public:
	TileDrawer(void);
	TileDrawer(const char* filepath);
	~TileDrawer(void);
	void Draw();
	void SetScroll(PositionInt2D& scroll);
	void SetScroll(int x,int y);
};

