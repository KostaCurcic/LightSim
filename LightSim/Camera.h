#pragma once

#include "Ray.h"

class Line;

class Camera
{
public:

	Point edgePoints[4];
	Line *edgeLines[4];

	double aspect;

	Camera();

	void drawRay(Ray& l, float r, float g, float b, float a);
	void drawLine(Line& l, float r, float g, float b, float a);

	void drawUnsafe(Line l, float r, float g, float b, float a);
	void drawUnsafe(Point* p, int n, float r, float g, float b, float a);

private:
};
