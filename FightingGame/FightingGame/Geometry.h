#pragma once


//�T�C�Y��\���\����
struct Size {
	int w;//��
	int h;//����
	Size() :w(), h() {}
	Size(int inw, int inh) :w(inw), h(inh) {}
};

//���W��\���\����
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


//�Ƃ肠�����u���W�v���ĈӖ����ƃx�N�^���
//Position�̂ق����悭�ˁH���ė��R�ł��̖��O
typedef Vector2 Position2;

///��`�\����
struct Rect {
	Position2 pos; //���S���W
	int w, h;//��,����
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

	///�����̋�`��`�悷��
	///@param color ��`�̐F(�f�t�H���g�͔�)
	void Draw(unsigned int color=0xffffffff);

	///�����̋�`��`�悷��(�I�t�Z�b�g�t��)
	///@param offset ��`���W�̃I�t�Z�b�g
	///@param ��`�̐F(�f�t�H���g�͔�)
	void Draw(const Vector2& offset,unsigned int color=0xffffffff);
};


