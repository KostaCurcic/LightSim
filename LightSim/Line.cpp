#include "Ray.h"
#include "Line.h"
#include "OpenGL3.1.h"
#include <cmath>


Line::Line(Point a, Point b) : Ray(a, b)
{
	l = Vector(b - a).Length();
}

Line::Line(Ray r, double l) : Ray(r)
{
	this->l = l;
}

const Point& Line::p1() const
{
	return o;
}

Point Line::p2() const
{
	return getPointFromT(l);
}

bool Line::isPointOn(const Point& p) const {
	Point p1v = p1();
	Point p2v = p2();
	return (p.x <= p1v.x == p.x >= p2v.x) && (p.y <= p1v.y == p.y >= p2v.y);
}

Vector Line::normal() const
{
	return Vector(d.y, -d.x);
}

Vector Line::normal(const Point& at)
{
	return normal();
}

void Line::draw(Camera& cam)
{
	cam.drawLine(*this, 0, 0, 0, 1);
}


Point Line::intersect(Ray& r)
{
	genLineFormula();
	r.genLineFormula();

	if (slope == r.slope) {
		if (offset == r.offset) {
			return o;
		}
		else return Point();
	}

	double x = (r.offset - offset) / (slope - r.slope);
	double y;
	if (slope > 99999)
		y = r.slope * x + r.offset;
	else
		y = slope * x + offset;
	Point i = Point(x, y);
	if (isPointOn(i) && r.isPointOn(i)) {
		return i;
	}
	return Point();
}
