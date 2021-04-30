#include "Camera.h"
#include "OpenGL3.1.h"
#include "Line.h"
#include <cmath>

Camera::Camera()
{
	aspect = 1.77777777777778;
	recalculateLines();
	pixels = new Color[sensorRes];
}

Camera::~Camera()
{
	delete pixels;
}

void Camera::drawRay(Ray& l, Color c)
{
	Point i1, i2;

	for (int i = 0; i < 4; i++) {
		if (edgeLines[i]->intersect(l, i1.isValid() ? &i2 : &i1, 0, 0) && i2.isValid()) {
			break;
		}
	}

	if (i1.isValid()) {
		if (!i2.isValid()) {
			drawUnsafe(Line(l.o, i1), c);
		}
		else {
			drawUnsafe(Line(i1, i2), c);
		}
	}
}

void Camera::drawUnsafe(Line l, Color c)
{
	Point p1 = l.p1();
	Point p2 = l.p2();
	glBegin(GL_LINES); {
		glColor4f(c.r, c.g, c.b, c.a);
		glVertex2d(p1.x, p1.y);
		glVertex2d(p2.x, p2.y);
	}
	glEnd();
}

void Camera::drawUnsafe(Point *p, int n, Point offset, Color c)
{
	glBegin(GL_LINE_STRIP); {
		glColor4f(c.r, c.g, c.b, c.a);
		for (int i = 0; i < n; i++) {
			glVertex2d(offset.x + p[i].x, offset.y + p[i].y);
		}
	}
	glEnd();
}

void Camera::renderSensor()
{
	float exp = 0;
	for (int i = 0; i < sensorRes; i++) {
		if (pixels[i].r > exp) exp = pixels[i].r;
		if (pixels[i].g > exp) exp = pixels[i].g;
		if (pixels[i].b > exp) exp = pixels[i].b;
	}

	exp /= 5;

	float invGamma = 1 / gamma;

	glLineWidth(senRenWidth);
	glBegin(GL_QUAD_STRIP); {
		for (int i = 0; i < sensorRes; i++) {
			glColor3f(powf(pixels[i].r / exp, invGamma), powf(pixels[i].g / exp, invGamma), powf(pixels[i].b / exp, invGamma));
			pixels[i] = Color();
			glVertex2d(-aspect * rScale + xPos, (-rScale + yPos) + (i / (float)sensorRes) * rScale * 2);
			glVertex2d(-aspect * rScale + xPos + senRenWidth * rScale, (-rScale + yPos) + (i / (float)sensorRes) * rScale * 2);
		}
		glVertex2d(-aspect * rScale + xPos, (-rScale + yPos) + rScale * 2);
		glVertex2d(-aspect * rScale + xPos + senRenWidth * rScale , (-rScale + yPos) + rScale * 2);
	} glEnd();
	glLineWidth(1);
}

void Camera::recalculateLines()
{
	edgePoints[0] = Point(-rScale * aspect + xPos, -rScale + yPos);
	edgePoints[1] = Point(rScale * aspect + xPos, -rScale + yPos);
	edgePoints[2] = Point(rScale * aspect + xPos, rScale + yPos);
	edgePoints[3] = Point(-rScale * aspect + xPos, rScale + yPos);
	edgeLines[0] = new Line(edgePoints[0], edgePoints[1]);
	edgeLines[1] = new Line(edgePoints[1], edgePoints[2]);
	edgeLines[2] = new Line(edgePoints[2], edgePoints[3]);
	edgeLines[3] = new Line(edgePoints[3], edgePoints[0]);
	edgeLines[0]->genLineFormula();
	edgeLines[1]->genLineFormula();
	edgeLines[2]->genLineFormula();
	edgeLines[3]->genLineFormula();
}

void Camera::rescaleWorld()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-aspect * rScale + xPos, aspect * rScale + xPos, -rScale + yPos, rScale + yPos, -1.0f, 1.0f);
	recalculateLines();
}
