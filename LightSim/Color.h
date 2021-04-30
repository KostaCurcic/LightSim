#pragma once

class Color
{
public:

	Color();
	Color(double r, double b, double g, double a);

	Color operator+(const Color&) const;
	Color operator-(const Color&) const;

	Color operator*(const double) const;
	Color operator/(const double) const;

	double r, g, b, a;

private:

};