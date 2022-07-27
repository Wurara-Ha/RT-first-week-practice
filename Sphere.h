#pragma once

#include "Core.h"
#include "Hittable.h"
#include "Material.h"

class Sphere : public Hittable
{
public:
	Sphere() {}
	Sphere(Point3 Cen, FLOAT_TYPE r, shared_ptr<Material> m) :Center(Cen), Radius(r), MatPtr(m) {}

	virtual bool Hit(const Ray& r, FLOAT_TYPE tMin, FLOAT_TYPE tMax, HitRecord& rec) const override;

private:
	Point3 Center;
	FLOAT_TYPE Radius;
	shared_ptr<Material> MatPtr;
};

bool Sphere::Hit(const Ray& r, FLOAT_TYPE tMin, FLOAT_TYPE tMax, HitRecord& rec) const
{
	Vec3 Oc = r.Origin() - Center;
	auto a = Dot(r.Direction(), r.Direction());
	auto half_b = Dot(Oc, r.Direction());
	auto c = Dot(Oc, Oc) - Radius * Radius;

	auto discriminant = half_b * half_b - a * c;
	if (discriminant < 0) return false;
	auto sqrtd = sqrt(discriminant);

	// Find the nearest root that lies in the acceptable range
	auto root = (-half_b - sqrtd) / a;
	if (root < tMin || root > tMax)
	{
		root = (-half_b + sqrtd) / a;
		if (root < tMin || root > tMax)
		{
			return false;
		}
	}

	rec.t = root;
	rec.p = r.At(root);
	Vec3 OutwardNormal = (rec.p - Center) / Radius;
	rec.SetFaceNormal(r, OutwardNormal);
	rec.MatPtr = MatPtr;

	return true;
}
