#include "../include/Vector2i.hpp"

Vector2i::Vector2i() {}

Vector2i::Vector2i(int xVal, int yVal)
{
	x = xVal;
	y = yVal;
}

int Vector2i::deltaDistance(const Vector2i& a, const Vector2i& b)
{
	int dx = a.x - b.x;
	if (dx < 0)
		dx = -dx;

	int dy = a.y - b.y;
	if (dy < 0)
		dy = -dy;

	return dx + dy;
}