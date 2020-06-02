#pragma once
#include "basedrawer.h"
#include<deque>

class LagDrawer : public BaseDrawer
{
	private:
		typedef std::deque<PositionInt2D> Positions_t;
		Positions_t _lagPositions;
		int _intervalFrame;
		int _frame;
		int _lagNum;
		bool _doShrink;
	public:
		LagDrawer(const char* filepath,int intervalFrame=60,int lagNum=10);
		~LagDrawer(void);
		void Draw();
};

