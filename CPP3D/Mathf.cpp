#include "Mathf.h"
#include <math.h>
#include "Vector2.h"

float Mathf::Clamp(float value, float min, float max) {
	if (value < min) return min;
	if (value > max) return max;
	return value;
}

float Mathf::Abs(float value) {
	return (value >= 0) ? value : -value;
}