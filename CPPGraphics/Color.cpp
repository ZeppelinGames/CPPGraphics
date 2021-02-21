#include "Color.h"
#include <cstdint>
#include "Mathf.h"
#include <math.h>

Color::Color() {
	red = 255;
	green = 255;
	blue = 255;
	alpha = 255;
}

Color::Color(unsigned char r, unsigned char g, unsigned char b) {
	red = r;
	green = g;
	blue = b;
	alpha = 255;
}

Color::Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
	red = r;
	green = g;
	blue = b;
	alpha = a;
}

#pragma region Preset Colors
Color Color::White = Color(255, 255, 255);
Color Color::Black = Color(0, 0, 0);
Color Color::Red = Color(255, 0, 0);
Color Color::Green =Color(0, 255, 0);
Color Color::Blue= Color(0, 0, 255);
Color Color::Yellow =Color(255, 255, 0);
Color Color::Orange =  Color(255, 69, 0);
Color Color::Purple = Color(255, 0, 255);
#pragma endregion

#pragma region Color operations with color

Color Color::operator+(Color c) {
	Color addedColor = Color(
		(int)Mathf::Clamp(red + c.red, 0, 255),
		(int)Mathf::Clamp(green + c.green, 0, 255),
		(int)Mathf::Clamp(blue + c.blue, 0, 255),
		(int)Mathf::Clamp(alpha + c.alpha, 0, 255)
	);
	return addedColor;
}

Color Color::operator-(Color c) {
	Color subtractedColor = Color(
		(int)Mathf::Clamp(red - c.red, 0, 255),
		(int)Mathf::Clamp(green - c.green, 0, 255),
		(int)Mathf::Clamp(blue - c.blue, 0, 255),
		(int)Mathf::Clamp(alpha - c.alpha, 0, 255)
	);
	return subtractedColor;
}

Color Color::operator*(Color c) {
	Color multipliedColor = Color(
		(int)Mathf::Clamp(red * c.red, 0, 255),
		(int)Mathf::Clamp(green * c.green, 0, 255),
		(int)Mathf::Clamp(blue * c.blue, 0, 255),
		(int)Mathf::Clamp(alpha * c.alpha, 0, 255)
	);
	return multipliedColor;
}

Color Color::operator/(Color c) {
	Color dividedColor = Color(
		(int)Mathf::Clamp(red / c.red, 0, 255),
		(int)Mathf::Clamp(green / c.green, 0, 255),
		(int)Mathf::Clamp(blue / c.blue, 0, 255),
		(int)Mathf::Clamp(alpha / c.alpha, 0, 255)
	);
	return dividedColor;
}

#pragma endregion

#pragma region Color operations with float
Color Color:: operator+(float i) {
	Color addedColor = Color(
		(int)Mathf::Clamp(red + i, 0, 255),
		(int)Mathf::Clamp(green + i, 0, 255),
		(int)Mathf::Clamp(blue + i, 0, 255),
		(int)Mathf::Clamp(alpha + i, 0, 255)
	);
	return addedColor;
}

Color Color:: operator-(float i) {
	Color subtractedColor = Color(
		(int)Mathf::Clamp(red - i, 0, 255),
		(int)Mathf::Clamp(green - i, 0, 255),
		(int)Mathf::Clamp(blue - i, 0, 255),
		(int)Mathf::Clamp(alpha - i, 0, 255)
	);
	return subtractedColor;
}

Color Color:: operator*(float i) {
	Color multipliedColor = Color(
		(int)Mathf::Clamp(red * i, 0, 255),
		(int)Mathf::Clamp(green * i, 0, 255),
		(int)Mathf::Clamp(blue * i, 0, 255),
		(int)Mathf::Clamp(alpha * i, 0, 255)
	);
	return multipliedColor;
}

Color Color:: operator/(float i) {
	Color dividedColor = Color(
		(int)Mathf::Clamp(red / i, 0, 255),
		(int)Mathf::Clamp(green / i, 0, 255),
		(int)Mathf::Clamp(blue / i, 0, 255),
		(int)Mathf::Clamp(alpha / i, 0, 255)
	);
	return dividedColor;
}

#pragma endregion

Color Color::UINTToColor(unsigned int c) {
	return Color(c >> 16,c >> 8,c);
}

unsigned char Color::GetRed(unsigned int c) {
	return c >> 16;
}

unsigned char Color::GetGreen(unsigned int c) {
	return c >> 8;
}

unsigned char Color::GetBlue(unsigned int c) {
	return c;
}

unsigned int Color::ColorToUINT(Color c) {
	int rgb = ((c.red & 0x0ff) << 16) | ((c.green & 0x0ff) << 8) | (c.blue & 0x0ff);
	return rgb;
}

