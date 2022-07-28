#pragma once

#define USE_RANDOM_LIB

#include <cmath>
#include <limits>
#include <memory>
#include <cstdlib>

#ifdef USE_RANDOM_LIB
#include <random>
#endif // USE_RANDOM_LIB


// Macros

#define FLOAT_TYPE float
#define EPSILON 1e-3

// Using

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

// Constants

const FLOAT_TYPE Inf = std::numeric_limits<FLOAT_TYPE>::infinity();
const FLOAT_TYPE pi = 3.1415926535897932385;

// Utility Functions

inline FLOAT_TYPE DegreesToRadians(FLOAT_TYPE degrees)
{
	return degrees * pi / 180;
}

#ifndef USE_RANDOM_LIB

inline FLOAT_TYPE RandomFloat()
{
	return rand() / (RAND_MAX + 1.0);
}

#else

inline FLOAT_TYPE RandomFloat()
{
	static std::uniform_real_distribution<FLOAT_TYPE> Distribution(0.0, 1.0);
	static std::mt19937 Generator;
	return Distribution(Generator);
}

#endif // !USE_RANDOM_LIB

inline FLOAT_TYPE RandomFloat(FLOAT_TYPE Min, FLOAT_TYPE Max)
{
	return Min + RandomFloat() * (Max - Min);
}

inline FLOAT_TYPE Clamp(FLOAT_TYPE x, FLOAT_TYPE Min, FLOAT_TYPE Max)
{
	if (x < Min) return Min;
	if (x > Max) return Max;
	return x;
}

inline int RandInt(int Min, int Max)
{
	// Return a random interger in [Min, Max]
	return static_cast<int>(RandomFloat(Min, Max + 0.99999));
}

// Common Headers

#include "Ray.h"
#include "Vec.h"
#include "Color.h"
