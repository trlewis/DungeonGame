#include "Vector2i.hpp"
#include <cmath>

Vector2i::Vector2i() {}

Vector2i::Vector2i(int xVal, int yVal)
{
	x = xVal;
	y = yVal;
}

int Vector2i::deltaDistance(const Vector2i& a, const Vector2i& b)
{
	return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}