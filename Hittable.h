#pragma once

#include "Core.h"
#include "Vec.h"
#include "Ray.h"
#include "AABB.h"

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
	virtual void ConstructBBox() = 0;

	bool TryGetBBox(AABB& _BBox) const;
	bool HasBBox() const;
protected:
	shared_ptr<AABB> BBox;
};

bool Hittable::TryGetBBox(AABB& _BBox) const
{
	if (HasBBox())
	{
		_BBox = *BBox.get();
		return true;
	}
	else
	{
		return false;
	}
}

bool Hittable::HasBBox() const
{
	return BBox ? true : false;
}
