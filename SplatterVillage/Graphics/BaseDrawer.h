#pragma once
#include "Drawer.h"

///描画用基本オブジェクト
///ロードして描画する以外には特に機能を持たない
class BaseDrawer : public Drawer
{
private:

	bool _adjustWindow;
protected:
	Size2D _size;
public:
	BaseDrawer(void);
	BaseDrawer(const char* filepath);
	virtual ~BaseDrawer(void);
	virtual void Load(const char* filepath);
	virtual void BeforeDraw(){}
	virtual void Draw();
	virtual void Draw(int alpha);
	virtual void Draw(PositionInt2D& pos);
	virtual void Draw(PositionInt2D& pos,int alpha);
	virtual void AfterDraw(){}
	virtual void SetLocation(GraphLocation loc);
	virtual void BlinkTime(unsigned int frame){};
	virtual void AdjustWindow();
};

