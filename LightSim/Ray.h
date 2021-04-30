#pragma once

#include "Vector.h"


class Ray
{
public:
	Ray();
	Ray(Point, Point);
	Ray(Point, Vector);

	virtual bool isPointOn(const Point&) const;
	virtual Ray& genLineFormula();

	Point getPointFromT(double t) const;

	Point o;
	Vector d;
	double slope, offset;

	double curRefIndex = 1.0f;

private:
};
