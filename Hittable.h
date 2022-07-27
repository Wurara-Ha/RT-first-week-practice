#pragma once

#include "Core.h"
#include "Vec.h"
#include "Ray.h"

class Material;

struct HitRecord
{
	Point3 p;
	Vec3 Normal;
	shared_ptr<Material> MatPtr;
	FLOAT_TYPE t;
	bool IsFrontFace;

	inline void SetFaceNormal(const Ray& r, const Vec3& OutwardNormal)
	{
		IsFrontFace = Dot(r.Direction(), OutwardNormal) < 0;
		Normal = IsFrontFace ? OutwardNormal : -OutwardNormal;
	}
};

class Hittable
{
public:
	virtual bool Hit(const Ray& r, FLOAT_TYPE tMin, FLOAT_TYPE tMax, HitRecord& rec) const = 0;
};
