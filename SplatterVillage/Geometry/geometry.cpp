#include"geometry.h"

VectorInt2D operator+(const VectorInt2D& v1 , const VectorInt2D& v2 ){
	return VectorInt2D(v1.x+v2.x,v1.y+v2.y);
}

VectorInt2D operator-(const VectorInt2D& v1 , const VectorInt2D& v2 ){
	return VectorInt2D(v1.x-v2.x,v1.y-v2.y);
}
