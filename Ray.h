#pragma once

#include "Core.h"
#include "Vec.h"

class Ray
{
public:
	Ray() {}
	Ray(const Point3& Origin, const Vec3& Direction) : Orig(Origin), Dir(Direction) {}

	Point3 Origin() const { return Orig; }
	Vec3 Direction() const { return Dir; }

	Point3 At(const FLOAT_TYPE t) const
	{
		return Orig + t * Dir;
	}

private:
	Point3 Orig;
	Vec3 Dir;
};
