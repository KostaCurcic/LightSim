#pragma once

#include "Line.h"

class Camera;

class FunctionalObj : public Line
{
public:

	FunctionalObj();
	FunctionalObj(Point, Point);
	FunctionalObj(Point, Point, double (*curveFun)(double));
	FunctionalObj(double (*curveFun)(double));

	~FunctionalObj();

	void draw(Camera& cam);
	Ray intersect(Ray&);
	Vector normal(const Point& at);

	double (*fun)(double);

	Point* pointBuff;

private:
	int res = 256;
	int divisions = 2;
	Ray intDivide(int min, int max, int size, Ray& r);
};