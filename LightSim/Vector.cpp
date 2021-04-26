#include "Vector.h"

#include <math.h>

Vector::Vector(const Point &p)
{
	x = p.x;
	y = p.y;
}

Vector& Vector::Unzero()
{
	if (x == 0.0f) {
		x = 0.0000001;
	}
	if (y == 0.0f) {
		y = 0.0000001;
	}
	return *this;
}

Vector & Vector::Normalize()
{
	double sum = Length();

	x /= sum;
	y /= sum;

	return *this;
}

double Vector::Length() const
{
	return sqrtf(x * x + y * y);
}

double Vector::operator*(const Vector &v) const
{
	return x * v.x + y * v.y;
}

Vector Vector::operator*(const double s) const
{
	return Vector(x * s, y * s);
}

Vector Vector::operator/(const double s) const
{
	return Vector(x / s, y / s);
}

Vector Vector::operator-() const
{
	return Vector(-x, -y);
}
Vector Vector::operator-(const Vector &p) const
{
	return Vector(x - p.x, y - p.y);
}

Vector Vector::Reflect(const Vector & normal) const
{
	return *this - (normal * (2 * (*this * normal)));
}

Vector Vector::Refract(const Vector & normal, double index) const
{
	index -= 1.0f;
	if (*this * normal < 0) index = -index;
	return *this + (normal * (index));
}
