#pragma once
class Vector2
{
public:
	float x, y;

	Vector2();
	Vector2(float, float);

	Vector2 operator + (Vector2);
	Vector2 operator - (Vector2);
	Vector2 operator * (Vector2);
	Vector2 operator / (Vector2);

	static float Distance(Vector2, Vector2);
	static Vector2 RotatePoint(Vector2, Vector2, float);

	
};

