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
	virtual Point intersect(Ray&) = 0;
	virtual Vector normal(const Point& at) = 0;

	Behavior behavior;
	double refInd;

private:

};
