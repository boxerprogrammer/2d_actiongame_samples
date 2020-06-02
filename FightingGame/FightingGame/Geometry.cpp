#include"Geometry.h"
#include<DxLib.h>


Vector2 operator+(const Vector2& lval, const Vector2& rval) {
	Vector2 ret = lval;
	ret.x += rval.x;
	ret.y += rval.y;
	return ret;
}

void
Rect::Draw(unsigned int color) {
	DxLib::DrawBox(Left()*2, Top()*2, Right()*2, Bottom()*2, color, false);
}

void
Rect::Draw(const Vector2& offset, unsigned int color) {
	DxLib::DrawBox((Left()+offset.x)*2, (Top()+offset.y)*2, (Right()+offset.x)*2, (Bottom()+offset.y)*2, color, false);
}
