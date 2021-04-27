#include "Camera.h"
#include "OpenGL3.1.h"
#include "Line.h"

Camera::Camera()
{
	aspect = 1.77777777777778;
	edgePoints[0] = Point(-1 * aspect, -1);
	edgePoints[1] = Point(1 * aspect, -1);
	edgePoints[2] = Point(1 *aspect, 1);
	edgePoints[3] = Point(-1 * aspect, 1);
	edgeLines[0] = new Line(edgePoints[0], edgePoints[1]);
	edgeLines[1] = new Line(edgePoints[1], edgePoints[2]);
	edgeLines[2] = new Line(edgePoints[2], edgePoints[3]);
	edgeLines[3] = new Line(edgePoints[3], edgePoints[0]);
}

void Camera::drawLine(Line& l, float r, float g, float b, float a)
{
	Point i1, i2;

	for (int i = 0; i < 4; i++) {
		Point in = edgeLines[i]->intersect((Ray&)l).o;
		if (in.isValid()) {
			if (!i1.isValid()) {
				i1 = in;
			}
			else if (!i2.isValid()) {
				i2 = in;
				break;
			}
		}
	}

	bool p1In = l.p1().x >= edgePoints[0].x && l.p1().x <= edgePoints[2].x && l.p1().y >= edgePoints[0].y && l.p1().y < edgePoints[2].y;

	if (!i1.isValid()) {
		if (p1In) {
			drawUnsafe(l, r, g, b, a);
		}
	}
	else if (!i2.isValid()) {
		if (p1In) {
			drawUnsafe(Line(l.p1(), i1), r, g, b, a);
		}
		else {
			drawUnsafe(Line(l.p2(), i1), r, g, b, a);
		}
	}
	else {
		drawUnsafe(Line(i1, i2), r, g, b, a);
	}
}

void Camera::drawRay(Ray& l, float r, float g, float b, float a)
{
	Point i1, i2;

	for (int i = 0; i < 4; i++) {
		Point in = edgeLines[i]->intersect(l).o;
		if (in.isValid()) {
			if (!i1.isValid()) {
				i1 = in;
			}
			else if (!i2.isValid()) {
				i2 = in;
				break;
			}
		}
	}

	if (i1.isValid()) {
		if (!i2.isValid()) {
			drawUnsafe(Line(l.o, i1), r, g, b, a);
		}
		else {
			drawUnsafe(Line(i1, i2), r, g, b, a);
		}
	}
}

void Camera::drawUnsafe(Line l, float r, float g, float b, float a)
{
	Point p1 = l.p1();
	Point p2 = l.p2();
	glBegin(GL_LINES); {
		glColor4f(r, g, b, a);
		glVertex2d(p1.x, p1.y);
		glVertex2d(p2.x, p2.y);
	}
	glEnd();
}

void Camera::drawUnsafe(Point *p, int n, float r, float g, float b, float a)
{
	glBegin(GL_LINE_STRIP); {
		glColor4f(r, g, b, a);
		for (int i = 0; i < n; i++) {
			glVertex2d(p[i].x, p[i].y);
		}
	}
	glEnd();
}
