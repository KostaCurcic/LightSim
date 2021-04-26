#pragma once

class Point
{
public:
	Point();
	Point(double, double);

	bool isValid();

	Point operator+(const Point&) const;
	Point operator-(const Point&) const;

	double x, y;

private:
};
