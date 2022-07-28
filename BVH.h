#pragma once

#include "Core.h"
#include "Hittable.h"
#include "HittableList.h"

#include <algorithm>

bool BoxXCompare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b);
bool BoxYCompare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b);
bool BoxZCompare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b);

class BvhNode : public Hittable
{
public:
	BvhNode() {}
	BvhNode(std::vector<shared_ptr<Hittable>>& SrcObjects, size_t Start, size_t End);
	BvhNode(HittableList& List):BvhNode(List.GetObjects(), 0, List.GetObjects().size()) { }

	virtual bool Hit(const Ray& r, FLOAT_TYPE tMin, FLOAT_TYPE tMax, HitRecord& rec) const override;
	virtual void ConstructBBox() override {}

private:
	shared_ptr<Hittable> Left;
	shared_ptr<Hittable> Right;
};

BvhNode::BvhNode(std::vector<shared_ptr<Hittable>>& SrcObjects, size_t Start, size_t End)
{
	// 捋一捋是否需要复制？应该需要
	std::vector<shared_ptr<Hittable>>& Objects = SrcObjects;

	int Axis = RandInt(0, 2);
	auto Comparator = (Axis == 0) ? BoxXCompare
					: (Axis == 1) ? BoxYCompare
								  : BoxZCompare;

	size_t ObjectSpan = End - Start;

	if (ObjectSpan == 1)
	{
		Left = Right = Objects[Start];
	}
	else if (ObjectSpan == 2)
	{
		if (Comparator(Objects[Start], Objects[Start + 1]))
		{
			Left = Objects[Start];
			Right = Objects[Start + 1];
		}
		else
		{
			Left = Objects[Start + 1];
			Right = Objects[Start];
		}
	}
	else
	{
		std::sort(Objects.begin() + Start, Objects.begin() + End, Comparator);

		auto Mid = Start + ObjectSpan / 2;
		Left = make_shared<BvhNode>(Objects, Start, Mid);
		Right = make_shared<BvhNode>(Objects, Mid, End);
	}

	AABB BBoxLeft, BBoxRight;
	Left->TryGetBBox(BBoxLeft);
	Right->TryGetBBox(BBoxRight);

	BBox = make_shared<AABB>(SurroundingBox(BBoxLeft, BBoxRight));
}

bool BvhNode::Hit(const Ray& r, FLOAT_TYPE tMin, FLOAT_TYPE tMax, HitRecord& rec) const
{
	if (!HasBBox() || !BBox->Hit(r, tMin, tMax))
	{
		return false;
	}
	bool HitLeft = Left->Hit(r, tMin, tMax, rec);
	bool HitRight = Right->Hit(r, tMin, HitLeft ? rec.t : tMax, rec);

	return HitLeft || HitRight;
}

inline bool BoxCompare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b, int Axis)
{
	AABB Boxa, Boxb;
	if (!a->TryGetBBox(Boxa) || !b->TryGetBBox(Boxb))
	{
		std::cerr << "No bounding box in BnvNode constructor" << std::endl;
	}

	return Boxa.Min()[Axis] < Boxb.Min()[Axis];
}

bool BoxXCompare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b)
{
	return BoxCompare(a, b, 0);
}

bool BoxYCompare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b)
{
	return BoxCompare(a, b, 1);
}

bool BoxZCompare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b)
{
	return BoxCompare(a, b, 2);
}

