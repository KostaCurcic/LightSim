#include "World.h"

World::World()
{
	lights.push_back(Point(50000, 0));
	lights.push_back(Point(50, 15));
	lights.push_back(Point(1.5, -0.5));
}

void World::draw()
{
	int c = 0;
	for (auto i = lights.begin(); i != lights.end(); i++) {
		drawLight(*i, c % 3 == 0, c % 3 == 1, c % 3 == 2, 0.2f);
		c++;
	}
	for (auto i = objects.begin(); i != objects.end(); i++) {
		(*i)->draw(camera);
	}
}

void World::bounceRay(Ray nr, float r, float g, float b, float a, Obj* src, int bounce)
{
	if (bounce >= bounceLimit) return;
	Point finalCol = Point();
	Vector finalNorm = Vector();
	double finalLen = INFINITY;
	Obj* colObj = nullptr;
	for (auto i = objects.begin(); i != objects.end(); i++) {
		if (*i == src) continue;
		Ray col = (*i)->intersect(nr);
		if (!col.o.isValid()) continue;
		double len = Vector(col.o - nr.o).Length();
		if(len < finalLen){
			finalLen = len;
			finalCol = col.o;
			finalNorm = col.d;
			colObj = *i;
		}
	}

	Vector newVec;
	Ray newRay;

	if (finalCol.isValid()) {
		Line drawLine = Line(nr.o, finalCol);
		camera.drawLine(drawLine, r, g, b, a);

		switch (colObj->behavior) {
		case reflect:
			newVec = nr.d.Reflect(finalNorm);
			newRay = Ray(finalCol, newVec);
			bounceRay(newRay, r, g, b, a, colObj, ++bounce);
			return;
		case refract:
			newVec = nr.d.Refract(finalNorm, colObj->refInd / nr.curRefIndex);
			newRay = Ray(finalCol, newVec);
			newRay.curRefIndex = colObj->refInd;
			bounceRay(newRay, r, g, b, a, colObj, ++bounce);
			return;
		case absorb:
		case sensor:
			//TODO sensor
		default:
			return;
		}
	}
	else {
		camera.drawRay(nr, r, g, b, a);
	}
}

void World::drawLight(const Point& src, float r, float g, float b, float a)
{
	for (double i = -lightRange; i < lightRange; i += lightStep) {
		Ray ray = Ray(src, Point(1, i));
		bounceRay(ray, r, g, b, a, nullptr, 0);
	}
}
