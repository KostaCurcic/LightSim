#pragma once

#include "Ray.h"
#include "Color.h"

class Line;

class Camera
{
public:

	Point edgePoints[4];
	Line *edgeLines[4];

	double aspect;
	int sensorRes = 1000;
	float senRenWidth = 0.1;
	float gamma = 2.224f;

	double xPos = 0;
	double yPos = 0;
	double rScale = 2;

	Color* pixels;

	Camera();
	~Camera();

	void drawRay(Ray& l, Color c);

	void drawUnsafe(Line l, Color c);
	void drawUnsafe(Point* p, int n, Point offset, Color c);

	void renderSensor();
	void recalculateLines();
	void rescaleWorld();

private:
};
