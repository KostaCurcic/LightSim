#include "Color.h"

Color::Color()
{
	r = g = b = a = 0;
}

Color::Color(double r, double g, double b, double a)
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}

Color Color::operator+(const Color& f) const
{
	return Color(r + f.r, g + f.g, b + f.b, a + f.a);
}

Color Color::operator-(const Color& f) const
{
	return Color(r - f.r, g - f.g, b - f.b, a - f.a);
}

Color Color::operator*(const double f) const
{
	return Color(r * f, g * f, b * f, a * f);
}

Color Color::operator/(const double f) const
{
	return Color(r / f, g / f, b / f, a / f);
}
