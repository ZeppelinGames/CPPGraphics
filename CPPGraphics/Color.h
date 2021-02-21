#pragma once
class Color
{
public:
	unsigned char red;
	unsigned char green;
	unsigned char blue;
	unsigned char alpha;

	Color();
	Color(unsigned char, unsigned char, unsigned char);
	Color(unsigned char, unsigned char, unsigned char, unsigned char);

	Color operator+ (Color);
	Color operator-(Color);
	Color operator*(Color);
	Color operator/(Color);

	Color operator+ (float);
	Color operator- (float);
	Color operator* (float);
	Color operator/ (float);

	static Color White;
	static Color Black;
	static Color Red;
	static Color Green;
	static Color Blue;
	static Color Yellow;
	static Color Orange;
	static Color Purple;

	static Color UINTToColor(unsigned int);
	static unsigned int ColorToUINT(Color);
	static unsigned char GetRed(unsigned int);
	static unsigned char GetGreen(unsigned int);
	static unsigned char GetBlue(unsigned int);
};

