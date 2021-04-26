#include "Point.h"
#include <cmath>

Point::Point()
{
	x = y = nan("");
}

Point::Point(double X, double Y)
{
	x = X;
	y = Y;
}

bool Point::isValid()
{
	return x == x;
}

Point Point::operator+(const Point &p) const
{
	return Point(x + p.x, y + p.y);
}

Point Point::operator-(const Point &p) const
{
	return Point(x - p.x, y - p.y);
}