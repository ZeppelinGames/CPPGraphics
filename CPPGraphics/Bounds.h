#pragma once
#include "Vector2.h"
class Bounds
{
public:
	Vector2 position;
	Vector2 size;

	Bounds();
	Bounds(Vector2, Vector2);

	static bool pointInBounds(Vector2, Bounds);
	static bool inBounds(Bounds, Bounds);
};

