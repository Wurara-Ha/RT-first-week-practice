#pragma once

#include "Core.h"

class Camera
{
public:

	Camera(
		Point3 LookFrom, 
		Point3 LookAt,
		Vec3 Vup,
		const FLOAT_TYPE& vfov, 
		const FLOAT_TYPE& _AspectRatio,
		const FLOAT_TYPE Aperture,
		const FLOAT_TYPE FocusDist
	):AspectRatio(_AspectRatio) {

		auto Theta = DegreesToRadians(vfov);
		auto h = tan(Theta / 2);
		auto ViewportHeight = 2.0 * h;
		auto ViewportWidth = AspectRatio * ViewportHeight;

		w = UnitVec(LookFrom - LookAt);
		u = UnitVec(Cross(Vup, w));
		v = Cross(w, u);

		Origin = LookFrom;
		Horizontal = FocusDist * ViewportWidth * u;
		Vertical = FocusDist * ViewportHeight * v;
		LowerLeftCorner = Origin - Horizontal / 2 - Vertical / 2 - FocusDist * w;

		LensRadius = Aperture / 2;
	}

	Ray GetRay(FLOAT_TYPE s, FLOAT_TYPE t) const
	{
		Vec3 rd = LensRadius * RandomInUnitDisk();
		Vec3 Offset = u * rd.X() + v * rd.Y();
		return Ray(Origin + Offset, 
			       LowerLeftCorner + s * Horizontal + t * Vertical - Origin - Offset);
	}

	FLOAT_TYPE GetAspectRatio() const { return AspectRatio; }

private:

	FLOAT_TYPE AspectRatio;
	Point3 Origin;
	Point3 LowerLeftCorner;
	Vec3 Horizontal;
	Vec3 Vertical;
	Vec3 u, v, w;
	FLOAT_TYPE LensRadius;

};

class CameraFix
{
public:

	CameraFix(
		const FLOAT_TYPE& vfov,
		const FLOAT_TYPE& _AspectRatio
	) :AspectRatio(_AspectRatio) {

		auto Theta = DegreesToRadians(vfov);
		auto h = tan(Theta / 2);
		auto ViewportHeight = 2.0 * h;
		auto ViewportWidth = AspectRatio * ViewportHeight;

		auto focal_length = 1.0;

		Origin = Point3(0, 0, 0);
		Horizontal = Vec3(ViewportWidth, 0.0, 0.0);
		Vertical = Vec3(0.0, ViewportHeight, 0.0);
		LowerLeftCorner = Origin - Horizontal / 2 - Vertical / 2 - Vec3(0, 0, focal_length);
	}

	Ray GetRay(FLOAT_TYPE s, FLOAT_TYPE t) const
	{
		return Ray(Origin, LowerLeftCorner + s * Horizontal + t * Vertical - Origin);
	}

	FLOAT_TYPE GetAspectRatio() const { return AspectRatio; }

private:

	FLOAT_TYPE AspectRatio;
	Point3 Origin;
	Point3 LowerLeftCorner;
	Vec3 Horizontal;
	Vec3 Vertical;

};
