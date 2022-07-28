#pragma once

#include "core.h"

#include <cmath>
#include <iostream>

using std::sqrt;

class Vec3
{
public:
	Vec3() : e{0, 0, 0} {}
	Vec3(FLOAT_TYPE e0, FLOAT_TYPE e1, FLOAT_TYPE e2) : e{e0, e1, e2} {}

	FLOAT_TYPE X() const { return e[0]; }
	FLOAT_TYPE Y() const { return e[1]; }
	FLOAT_TYPE Z() const { return e[2]; }

	Vec3 operator-() const { return Vec3(-e[0], -e[1], -e[2]); }
	FLOAT_TYPE operator[](int i) const { return e[i]; }
	FLOAT_TYPE& operator[](int i) { return e[i]; }

	Vec3& operator+=(const Vec3& v)
	{
		e[0] += v.e[0];
		e[1] += v.e[1];
		e[2] += v.e[2];
		return *this;
	}

	Vec3& operator*=(const FLOAT_TYPE t)
	{
		e[0] *= t;
		e[1] *= t; 
		e[2] *= t;
		return *this;
	}

	Vec3& operator/=(const FLOAT_TYPE t)
	{
		return *this *= 1 / t;
	}

	FLOAT_TYPE Length() const
	{
		return sqrt(LengthSquared());
	}

	FLOAT_TYPE LengthSquared() const
	{
		return e[0]*e[0] + e[1] * e[1] + e[2] * e[2];
	}

	inline static Vec3 Random()
	{
		return Vec3(RandomFloat(), RandomFloat(), RandomFloat());
	}

	inline static Vec3 Random(FLOAT_TYPE Min, FLOAT_TYPE Max)
	{
		return Vec3(RandomFloat(Min, Max), RandomFloat(Min, Max), RandomFloat(Min, Max));
	}

	bool NearZero() const
	{
		return (fabs(e[0]) < EPSILON &&
				fabs(e[1]) < EPSILON &&
				fabs(e[2]) < EPSILON);
	}

private:
	FLOAT_TYPE e[3];
};

inline std::ostream& operator<<(std::ostream& os, const Vec3& vec)
{
	return os << "(" << vec.X() << "," << vec.Y() << "," << vec.Z() << ")" << std::endl;
}

inline Vec3 operator+(const Vec3& u, const Vec3& v)
{
	return Vec3(u.X() + v.X(), u.Y() + v.Y(), u.Z() + v.Z());
}

inline Vec3 operator-(const Vec3& u, const Vec3& v)
{
	return Vec3(u.X() - v.X(), u.Y() - v.Y(), u.Z() - v.Z());
}

inline Vec3 operator*(const Vec3& u, const Vec3& v)
{
	return Vec3(u.X() * v.X(), u.Y() * v.Y(), u.Z() * v.Z());
}

inline Vec3 operator*(const FLOAT_TYPE t, const Vec3& u)
{
	return Vec3(t * u.X(), t * u.Y(), t * u.Z());
}

inline Vec3 operator*(const Vec3& u, const FLOAT_TYPE t)
{
	return t * u;
}

inline Vec3 operator/(const Vec3& u, const FLOAT_TYPE t)
{
	return (1/t) * u;
}

inline FLOAT_TYPE Dot(const Vec3& u, const Vec3& v)
{
	return u.X() * v.X() + u.Y() * v.Y() + u.Z() * v.Z();
}

inline Vec3 Cross(const Vec3& u, const Vec3& v)
{
	return Vec3(u.Y() * v.Z() - u.Z() * v.Y(),
		        u.Z() * v.X() - u.X() * v.Z(),
		        u.X() * v.Y() - u.Y() * v.X());
}

inline Vec3 UnitVec(const Vec3& v)
{
	return v / v.Length();
}

Vec3 RandomInUnitSphere()
{
	while (true)
	{
		auto p = Vec3::Random(-1, 1);
		if (p.LengthSquared() >= 1) continue;
		return p;
	}
}

Vec3 RandomUnitVector()
{
	return UnitVec(RandomInUnitSphere());
}

Vec3 RandomInHemisphere(const Vec3& Normal)
{
	Vec3 InUnitSphere = RandomInUnitSphere();
	if (Dot(Normal, InUnitSphere) > 0.0)
		return InUnitSphere;
	else
		return -InUnitSphere;
}

Vec3 Reflect(const Vec3& In, const Vec3& Normal)
{
	return In - 2 * Dot(In, Normal) * Normal;
}

Vec3 Refract(const Vec3& uv, const Vec3& Normal, FLOAT_TYPE EtaiOverEtat)
{
	auto CosTheta = fmin(Dot(-uv, Normal), 1.0);
	Vec3 rOutPerp = EtaiOverEtat * (uv + CosTheta * Normal);
	Vec3 rOutParallel = -sqrt(fabs(1.0 - rOutPerp.LengthSquared())) * Normal;
	return rOutPerp + rOutParallel;
}

Vec3 RandomInUnitDisk()
{
	while (true)
	{
		auto p = Vec3(RandomFloat(-1, 1), RandomFloat(-1, 1), 0);
		if (p.LengthSquared() >= 1) continue;
		return p;
	}
}

using Point3 = Vec3;
using Color = Vec3;