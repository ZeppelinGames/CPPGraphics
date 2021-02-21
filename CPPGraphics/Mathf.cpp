#include "Mathf.h"
#include <math.h>
#include "Vector2.h"

/// <summary>
/// Clamp float value in range
/// </summary>
/// <param name="value">Value to clamp</param>
/// <param name="min">Min</param>
/// <param name="max">Max</param>
/// <returns>Clamped value</returns>
float Mathf::Clamp(float value, float min, float max) {
	if (value < min) return min;
	if (value > max) return max;
	return value;
}

/// <summary>
/// Returns absolute value
/// </summary>
/// <param name="value"></param>
/// <returns>absolute value</returns>
float Mathf::Abs(float value) {
	return (value >= 0) ? value : -value;
}