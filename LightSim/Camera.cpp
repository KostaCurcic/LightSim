#include "Camera.h"
#include "OpenGL3.1.h"
#include "Line.h"

Camera::Camera()
{
	edgeLines[0] = new Line(edgePoints[0], edgePoints[1]);
	edgeLines[1] = new Line(edgePoints[1], edgePoints[2]);
	edgeLines[2] = new Line(edgePoints[2], edgePoints[3]);
	edgeLines[3] = new Line(edgePoints[3], edgePoints[0]);
}

void Camera::drawLine(Line& l, float r, float g, float b, float a)
{
	Point i1, i2;

	for (int i = 0; i < 4; i++) {
		Point in = edgeLines[i]->intersect((Ray&)l);
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

	bool p1In = l.p1().x >= -1 && l.p1().x <= 1 && l.p1().y >= -1 && l.p1().y < 1;

	if (!i1.isValid()) {
		if (p1In) {
			draw(l, r, g, b, a);
		}
	}
	else if (!i2.isValid()) {
		if (p1In) {
			draw(Line(l.p1(), i1), r, g, b, a);
		}
		else {
			draw(Line(l.p2(), i1), r, g, b, a);
		}
	}
	else {
		draw(Line(i1, i2), r, g, b, a);
	}
}

void Camera::drawRay(Ray& l, float r, float g, float b, float a)
{
	Point i1, i2;

	for (int i = 0; i < 4; i++) {
		Point in = edgeLines[i]->intersect(l);
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
			draw(Line(l.o, i1), r, g, b, a);
		}
		else {
			draw(Line(i1, i2), r, g, b, a);
		}
	}
}

void Camera::draw(Line l, float r, float g, float b, float a)
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
