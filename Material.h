#pragma once

#include "Core.h"

struct HitRecord;

class Material
{
public:
	virtual bool Scatter(const Ray& rIn, const HitRecord& rec, Color& Attenuation, Ray& Scattered) const = 0;
};

class Lambertian : public Material
{
public:
	Lambertian(const Color& a):Albedo(a) {}

	virtual bool Scatter(const Ray& rIn, const HitRecord& rec, Color& Attenuation, Ray& Scattered) const override
	{
		auto ScatterDir = rec.Normal + RandomUnitVector();

		if (ScatterDir.NearZero())
		{
			ScatterDir = rec.Normal;
		}

		Scattered = Ray(rec.p, ScatterDir);
		Attenuation = Albedo;
		return true;
	}

private:
	Color Albedo;
};

class Metal : public Material
{
public:
	Metal(const Color& a, FLOAT_TYPE f):Albedo(a), Fuzz(f < 1 ? f : 1) {}

	virtual bool Scatter(const Ray& rIn, const HitRecord& rec, Color& Attenuation, Ray& Scattered) const override
	{
		auto ReflectDir = Reflect(UnitVec(rIn.Direction()), rec.Normal);
		Scattered = Ray(rec.p, ReflectDir + Fuzz * RandomInUnitSphere());
		Attenuation = Albedo;
		return (Dot(Scattered.Direction(), rec.Normal) > 0);
	}

private:
	Color Albedo;
	FLOAT_TYPE Fuzz;
};

class Dielectric : public Material
{
public:
	Dielectric(FLOAT_TYPE IndexOfRefraction):Ir(IndexOfRefraction) {}

	virtual bool Scatter(const Ray& rIn, const HitRecord& rec, Color& Attenuation, Ray& Scattered) const override
	{
		Attenuation = Color(1.0, 1.0, 1.0);
		FLOAT_TYPE RefractionRatio = rec.IsFrontFace ? (1.0 / Ir) : Ir;

		Vec3 UnitDirection = UnitVec(rIn.Direction());

		FLOAT_TYPE CosTheta = fmin(Dot(-rIn.Direction(), rec.Normal), 1.0);
		FLOAT_TYPE SinTheta = sqrt(1.0 - CosTheta * CosTheta);

		bool CanRefract = RefractionRatio * SinTheta < 1.0;
		Vec3 Direction;

		// 第二个判断条件用于空心玻璃球的渲染，不懂
		if (CanRefract && Reflectance(CosTheta, RefractionRatio) < RandomFloat())
		{
			Direction = Refract(UnitDirection, rec.Normal, RefractionRatio);
		}
		else
		{
			Direction = Reflect(UnitDirection, rec.Normal);
		}

		Scattered = Ray(rec.p, Direction);
		return true;
	}

private:
	FLOAT_TYPE Ir; // Index of Refraction

private:
	static FLOAT_TYPE Reflectance(FLOAT_TYPE Cosine, FLOAT_TYPE RefIdx)
	{
		// Use Schlick's Approximation for reflectance
		auto R0 = (1 - RefIdx) / (1 + RefIdx);
		R0 *= R0;
		return R0 + (1 - R0) * pow((1 - Cosine), 5);
	}
};
