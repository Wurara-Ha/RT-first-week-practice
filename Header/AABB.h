#pragma once

#include "Core.h"

class AABB
{
public:

	AABB() {}
	AABB(const AABB& _BBox) { Minimum = _BBox.Minimum; Maximum = _BBox.Maximum; }
	AABB(const Point3& A, const Point3& B):Minimum(A), Maximum(B) {}

	Point3 Min() const { return Minimum; }
	Point3 Max() const { return Maximum; }

	bool Hit(const Ray& r, FLOAT_TYPE tMin, FLOAT_TYPE tMax) const;

private:
	Point3 Minimum;
	Point3 Maximum;
};

bool AABB::Hit(const Ray& r, FLOAT_TYPE tMin, FLOAT_TYPE tMax) const
{
	for (int a = 0; a < 3; a++)
	{
		FLOAT_TYPE InvD = 1.0 / r.Direction()[a];
		FLOAT_TYPE t0 = (Min()[a] - r.Origin()[a]) * InvD;
		FLOAT_TYPE t1 = (Max()[a] - r.Origin()[a]) * InvD;

		if (InvD < 0.0)
		{
			std::swap(t0, t1);
		}
		tMin = t0 > tMin ? t0 : tMin;
		tMax = t1 < tMax ? t1 : tMax;

		if (tMin >= tMax)
		{
			return false;
		}
	}
	return true;
}

AABB SurroundingBox(AABB Box0, AABB Box1) {
	Point3 Small(fmin(Box0.Min().X(), Box1.Min().X()),
			     fmin(Box0.Min().Y(), Box1.Min().Y()),
			     fmin(Box0.Min().Z(), Box1.Min().Z()));

	Point3 Big(fmax(Box0.Max().X(), Box1.Max().X()),
			   fmax(Box0.Max().Y(), Box1.Max().Y()),
			   fmax(Box0.Max().Z(), Box1.Max().Z()));

	return AABB(Small, Big);
}
