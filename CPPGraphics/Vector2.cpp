#include "Vector2.h"
#include "Mathf.h"
#include <math.h> 

Vector2::Vector2() {
	x = 0;
	y = 0;
}

Vector2::Vector2(float newX, float newY) {
	x = newX;
	y = newY;
}

Vector2 Vector2::operator+ (Vector2 add) {
	return Vector2(x + add.x, y + add.y);
}

Vector2 Vector2::operator- (Vector2 sub) {
	return Vector2(x - sub.x, y - sub.y);
}

Vector2 Vector2::operator* (Vector2 multiply) {
	return Vector2(x * multiply.x, y * multiply.y);
}

Vector2 Vector2:: operator/ (Vector2 div) {
	return Vector2(x / div.x, y / div.y);
}

/// <summary>
/// Get distance between two points
/// </summary>
/// <param name="p1">Point 1</param>
/// <param name="p2">Point 2</param>
/// <returns>Distance between points</returns>
float Vector2::Distance(Vector2 p1, Vector2 p2) {
	float dist = abs((int)sqrt(pow(abs((int)((double)p2.y - (double)p1.y)), 2) + pow(abs(int((double)p2.x - (double)p1.x)), 2)));
	return dist;
}

/// <summary>
/// Rotates point p around center by angle
/// </summary>
/// <param name="center">Center point</param>
/// <param name="p">Point to rotate</param>
/// <param name="angle">Angle to rotate by</param>
/// <returns>New point position</returns>
Vector2 Vector2::RotatePoint(Vector2 center, Vector2 p, float angle) {
	float s = sin(angle);
	float c = cos(angle);

	// translate point back to origin:
	p.x -= center.x;
	p.y -= center.y;

	// rotate point
	float xnew = p.x * c - p.y * s;
	float ynew = p.x * s + p.y * c;

	// translate point back:
	p.x = xnew + center.x;
	p.y = ynew + center.y;
	return p;
}