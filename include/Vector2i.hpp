#ifndef VECTOR_2I_HPP_
#define VECTOR_2I_HPP_

class Vector2i
{
public:
	int x;
	int y;

	Vector2i();
	Vector2i(int xVal, int yVal);

	//! Gets the dx+dy distance between two vectors.
	/*! This method does NOT get the pythagorean/shortest distance. */
	static int deltaDistance(const Vector2i& a, const Vector2i& b);
};

#endif //VECTOR_2I_HPP_