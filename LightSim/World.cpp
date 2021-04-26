#include "World.h"

World::World()
{
	aspect = 1;
	lights.push_back(Point(10000000, 0));
	//lights.push_back(Point(50, 45));
	//lights.push_back(Point(1.5, -0.5));
}

void World::draw()
{
	int c = 0;
	for (auto i = lights.begin(); i != lights.end(); i++) {
		drawLight(*i, c % 3 == 0, c % 3 == 1, c % 3 == 2, 0.4f);
		c++;
	}
	for (auto i = objects.begin(); i != objects.end(); i++) {
		(*i)->draw(camera);
	}
}

void World::bounceRay(Ray nr, float r, float g, float b, float a, Obj* src, int bounce)
{
	Point finalCol = Point();
	double finalLen = INFINITY;
	Obj* colObj = nullptr;
	for (auto i = objects.begin(); i != objects.end(); i++) {
		if (*i == src) continue;
		Point col = (*i)->intersect(nr);
		if (!col.isValid()) continue;
		double len = Vector(col - nr.o).Length();
		if(len < finalLen){
			finalLen = len;
			finalCol = col;
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
			newVec = nr.d.Reflect(colObj->normal(finalCol));
			newRay = Ray(finalCol, newVec);
			bounceRay(newRay, r, g, b, a, colObj, ++bounce);
			return;
		case refract:
			newVec = nr.d.Refract(colObj->normal(finalCol), colObj->refInd / nr.curRefIndex);
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
