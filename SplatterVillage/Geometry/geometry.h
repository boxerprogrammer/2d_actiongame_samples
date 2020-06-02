#pragma once

struct Vector3D{
	Vector3D():x(0.f),y(0.f),z(0.f){}
	Vector3D(float ix,float iy, float iz):x(ix),y(iy),z(iz){}
	float x;
	float y;
	float z;
};
typedef Vector3D Position3D;

template<typename T>
struct Vector2D{
	Vector2D():x(0),y(0){}
	Vector2D(T ix,T iy):x(ix),y(iy){}
	T x;
	T y;
	void operator+=(const Vector2D& vec){
		x+=vec.x;
		y+=vec.y;
	}
};

typedef Vector2D<int> VectorInt2D;


VectorInt2D operator+(const VectorInt2D& v1 , const VectorInt2D& v2 );

VectorInt2D operator-(const VectorInt2D& v1 , const VectorInt2D& v2 );

typedef VectorInt2D PositionInt2D;

struct Size2D{
	Size2D():w(0),h(0){}
	Size2D(int iw,int ih):w(iw),h(ih){}
	int w;
	int h;
};

struct Rect{
private:
	bool _available;
public:
	Rect():center(),size(),_available(true){}
	Rect(int x,int y, int w, int h ){
		center.x=x;
		center.y=y;
		size.w=w;
		size.h=h;
		_available=true;
	}
	void On(){_available=true;}
	void Off(){_available=false;}
	PositionInt2D center;
	Size2D size;
	const int Left() const{
		return center.x-size.w;
	}
	const int Right() const{
		return center.x+size.w;
	}
	const int Top() const{
		return center.y-size.h;
	}
	const int Bottom() const{
		return center.y+size.h;
	}
	bool IsValid() const{
		return _available && (size.w!=0);
	}

};