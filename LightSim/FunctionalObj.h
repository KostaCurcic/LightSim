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
	bool intersect(Ray& r, Point* col, Vector* normal, double* per);
	Vector normal(const Point& at);

	double (*fun)(double);

	Point* pointBuff;

private:
	int res = 100000;
	int divisions = 2;
	void intDivide(int min, int max, int size, Ray& r, Point *col, Vector *normal, double *per);
};