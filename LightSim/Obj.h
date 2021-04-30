#pragma once

#include "Camera.h"

class Camera;

enum Behavior
{
	absorb,
	reflect,
	refract,
	sensor
};

class Obj
{
public:
	virtual void draw(Camera& cam) = 0;
	//Return contains ray with origin on intersection point, and direction of colision surface normal
	virtual bool intersect(Ray& r, Point *col, Vector *normal, double *per) = 0;
	virtual Vector normal(const Point& at) = 0;

	Behavior behavior;
	double refInd;

private:

};
