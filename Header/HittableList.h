#pragma once

#include "Hittable.h"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class HittableList : public Hittable
{
public:
	HittableList() {}
	HittableList(shared_ptr<Hittable> Object) { Add(Object); }

	void Clear() { Objects.clear(); }
	void Add(shared_ptr<Hittable> Object) { Objects.push_back(Object); }
	std::vector<shared_ptr<Hittable>>& GetObjects() { return Objects; }

	virtual bool Hit(const Ray& r, FLOAT_TYPE tMin, FLOAT_TYPE tMax, HitRecord& rec) const override;
	virtual void ConstructBBox() override {}

private:
	std::vector<shared_ptr<Hittable>> Objects;
};

// Get the closest hit location of objects, false if not hit anything
bool HittableList::Hit(const Ray& r, FLOAT_TYPE tMin, FLOAT_TYPE tMax, HitRecord& rec) const
{
	HitRecord TempRec;
	bool HitAnything = false;
	auto ClosestSoFar = tMax;

	// Intuitive hit implementation
	for (const auto& Object : Objects)
	{
		if (Object->Hit(r, tMin, ClosestSoFar, TempRec))
		{
			HitAnything = true;
			ClosestSoFar = TempRec.t;
			rec = TempRec;
		}
	}

	// Bvh hit implementation


	return HitAnything;
}