#include "CollisionDetector.h"
#include"Geometry.h"
#include<algorithm>
#include"CollidableActor.h"

CollisionDetector::CollisionDetector()
{
}


CollisionDetector::~CollisionDetector()
{
}

bool
CollisionDetector::IsHit(const Rect& rcA, const Rect& rcB) {
	return !(rcA.Right()<rcB.Left() ||
		rcB.Right()<rcA.Left() ||
		rcA.Bottom()<rcB.Top() ||
		rcB.Bottom()<rcA.Top());
}

bool
CollisionDetector::IsHit(const HitRect& rcA, const HitRect& rcB) {
	return !(rcA.GetRect().Right()<rcB.GetRect().Left() ||
		rcB.GetRect().Right()<rcA.GetRect().Left() ||
		rcA.GetRect().Bottom()<rcB.GetRect().Top() ||
		rcB.GetRect().Bottom()<rcA.GetRect().Top());
}

Position2 
CollisionDetector::OverlappedCenter(const Rect& rcA, const Rect& rcB) {
	int left = std::max(rcA.Left(), rcB.Left());
	int right = std::min(rcA.Right(), rcB.Right());
	int top = std::max(rcA.Top(), rcB.Top());
	int bottom = std::min(rcA.Bottom(), rcB.Bottom());
	return Position2((left + right) / 2, (top + bottom )/ 2);
}