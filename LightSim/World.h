#pragma once

#include "Line.h"
#include "Camera.h"
#include "Obj.h"
#include <vector>

class World
{
public:

	std::vector<Point> lights;
	std::vector<Obj*> objects;

	double aspect;
	double lightRange = 2;
	double lightStep = 0.02;
	Camera camera;

	int bounceLimit = 20;

	World();

	void draw();

private:
	void bounceRay(Ray nr, float r, float g, float b, float a, Obj *src, int bounce);
	void drawLight(const Point& src, float r, float g, float b, float a);
};