#pragma once


//サイズを表す構造体
struct Size {
	int w;//幅
	int h;//高さ
	Size() :w(), h() {}
	Size(int inw, int inh) :w(inw), h(inh) {}
};

//座標を表す構造体
struct Vector2 {
	Vector2():x(0),y(0){}
	Vector2(int inx,int iny):x(inx),y(iny){}
	int x, y;
	void operator+=(const Vector2& invec) {
		x += invec.x;
		y += invec.y;
	}
};

Vector2 operator+(const Vector2& lval, const Vector2& rval); 


//とりあえず「座標」って意味だとベクタより
//Positionのほうがよくね？って理由でこの名前
typedef Vector2 Position2;

///矩形構造体
struct Rect {
	Position2 pos; //中心座標
	int w, h;//幅,高さ
	Rect() : pos(0, 0), w(0), h(0) {}
	Rect(int x, int y, int inw, int inh) :
		pos(x, y), w(inw), h(inh) {}
	Rect(Position2& inpos, int inw, int inh) :
		pos(inpos), w(inw), h(inh)
	{}
	void SetCenter(int x, int y) {
		pos.x = x;
		pos.y = y;
	}
	void SetCenter(const Position2& inpos) {
		pos.x = inpos.x;
		pos.y = inpos.y;
	}
	Vector2 Center() {
		return pos;
	}
	int Left()const { return pos.x - w / 2; }
	int Top() const{ return pos.y - h / 2; }
	int Right()const  { return pos.x + w / 2; }
	int Bottom()const  { return pos.y + h / 2; }

	///自分の矩形を描画する
	///@param color 矩形の色(デフォルトは白)
	void Draw(unsigned int color=0xffffffff);

	///自分の矩形を描画する(オフセット付き)
	///@param offset 矩形座標のオフセット
	///@param 矩形の色(デフォルトは白)
	void Draw(const Vector2& offset,unsigned int color=0xffffffff);
};


