#pragma once

#include "Line.h"
#include "Camera.h"
#include "Color.h"
#include "Obj.h"
#include <vector>

class World
{
public:

	std::vector<Point> lights;
	std::vector<Obj*> objects;

	double lightRange = 2;
	double lightStep = 0.005;
	Camera camera;

	int bounceLimit = 10;

	World();

	void draw();

private:
	void bounceRay(Ray nr, Color c, Obj *src, int bounce);
	void drawLight(const Point& src, Color c);
};
