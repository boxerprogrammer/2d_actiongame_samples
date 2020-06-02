#pragma once
#include"Geometry.h"

class HitRect;

class CollisionDetector
{
public:
	CollisionDetector();
	~CollisionDetector();
	static bool IsHit(const Rect& rcA,const Rect& rcB);
	static bool IsHit(const HitRect& rcA,const  HitRect& rcB);
	static Position2 OverlappedCenter(const Rect& rcA, const Rect& rcB);
};

