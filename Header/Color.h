#pragma once

#include "Core.h"

#include <iostream>

void WriteColor(std::ostream& os, Color PixelColor, int SamplesPerPixel)
{
	auto r = PixelColor.X();
	auto g = PixelColor.Y();
	auto b = PixelColor.Z();

	// Divide the color by the number of samples and gamma-correct for gamma=2.0.
	auto Scale = 1.0 / SamplesPerPixel;
	r = sqrt(Scale * r);
	g = sqrt(Scale * g);
	b = sqrt(Scale * b);

	os << static_cast<short>(255.999 * Clamp(r, 0.0, 0.999)) << ' '
		<< static_cast<short>(255.999 * Clamp(g, 0.0, 0.999)) << ' '
		<< static_cast<short>(255.999 * Clamp(b, 0.0, 0.999)) << std::endl;
}
