#include "Ray.h"

#include <math.h>

Ray::Ray()
{
	o = Point();
	d = Vector();
}

Ray::Ray(Point p1, Point p2)
{
	o = p1;

	Vector vec = p2 - p1;
	d = vec.Normalize();
}

Ray::Ray(Point p1, Vector vec)
{
	o = p1;
	d = vec.Normalize();
}

bool Ray::isPointOn(const Point& p) const
{
	return (p.x != 0 && ((d.x < 0) == (p.x < o.x))) || (p.x == 0 && ((d.y < 0) == (p.y < o.y)));
}

void Ray::genLineFormula()
{
	if (d.x == 0) slope = 999999999999999999999999999999999999999.0;
	else slope = d.y / d.x;
	offset = o.y - slope * o.x;
}

Point Ray::getPointFromT(double t) const
{
	return o + d * t;
}
