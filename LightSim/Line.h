#pragma once
#include "Ray.h"
#include "Obj.h"

class Camera;

class Line : public Ray, public Obj
{
public:
	Line() : Ray() {};
	Line(Point, Point);
	Line(Ray, double);

	const Point& p1() const;
	Point p2() const;

	bool isPointOn(const Point& p) const;
	Vector normal() const;
	Vector normal(const Point& at);
	void draw(Camera& cam);

	Line& genLineFormula();

	bool intersect(Ray& r, Point* col, Vector* normal, double* per);

	double length;

private:

};

