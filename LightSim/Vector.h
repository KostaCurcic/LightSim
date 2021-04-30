#pragma once

#include "Point.h"

class Vector : public Point
{
public:
	Vector(const Point&);
	Vector() : Point() {};
	Vector(double X, double Y) : Point(X, Y) {};
	Vector& Unzero();
	Vector& Normalize();
	Vector Normal() const;
	double Length() const;
	double operator*(const Vector&) const;
	Vector operator*(const double) const;
	Vector operator/(const double) const;
	Vector operator-() const;
	Vector operator-(const Vector&) const;
	Vector Reflect(const Vector& normal) const;
	Vector Refract(const Vector& normal, double index) const;

private:

};
