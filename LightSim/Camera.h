#pragma once

#include "Ray.h"

class Line;

class Camera
{
public:

	Point edgePoints[4] = { Point(-1, -1), Point(-1, 1), Point(1, 1), Point(1, -1) };
	Line *edgeLines[4];

	Camera();

	void drawRay(Ray& l, float r, float g, float b, float a);
	void drawLine(Line& l, float r, float g, float b, float a);

private:
	void draw(Line l, float r, float g, float b, float a);
};
