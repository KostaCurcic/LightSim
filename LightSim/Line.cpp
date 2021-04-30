#include "Ray.h"
#include "Line.h"
#include "OpenGL3.1.h"
#include <cmath>


Line::Line(Point a, Point b) : Ray(a, b)
{
	length = Vector(b - a).Length();
}

Line::Line(Ray r, double l) : Ray(r)
{
	this->length = l;
}

const Point& Line::p1() const
{
	return o;
}

Point Line::p2() const
{
	return getPointFromT(length);
}

bool Line::isPointOn(const Point& p) const {
	Point p1v = p1();
	Point p2v = p2();
	return ((float)p.x <= (float)p1v.x == (float)p.x >= (float)p2v.x) && ((float)p.y <= (float)p1v.y == (float)p.y >= (float)p2v.y);
}

Vector Line::normal() const
{
	return d.Normal();
}

Vector Line::normal(const Point& at)
{
	return normal();
}

void Line::draw(Camera& cam)
{
	cam.drawUnsafe(*this, Color(0, 0, 0, 1));
}

Line& Line::genLineFormula()
{
	Ray::genLineFormula();
	return *this;
}


bool Line::intersect(Ray& r, Point* col, Vector* normal, double* per)
{
	genLineFormula();
	if (slope == r.slope) {
		if (offset == r.offset) {
			*col = o;
			if (normal) *normal = this->normal();
			if (per) *per = 0;
			return true;
		}
		else return false;
	}

	double x = (r.offset - offset) / (slope - r.slope);
	double y;
	if (slope > 999)
		y = r.slope * x + r.offset;
	else
		y = slope * x + offset;
	Point i = Point(x, y);
	if (isPointOn(i) && r.isPointOn(i)) {
		*col = i;
		if (normal) *normal = this->normal();
		if (per) *per = Vector(i - p1()).Length() / Vector(p2() - p1()).Length();
		return true;
	}
	return false;
}
