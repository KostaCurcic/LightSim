#include "World.h"

World::World()
{
	lights.push_back(Point(5000, 0));
	//lights.push_back(Point(50, 15));
	//lights.push_back(Point(1.5, -0.5));
}

void World::draw()
{
	int c = 0;
	for (auto i = lights.begin(); i != lights.end(); i++) {
		drawLight(*i, Color(c % 3 == 0, c % 3 == 1, c % 3 == 2, 0.4));
		c++;
	}
	for (auto i = objects.begin(); i != objects.end(); i++) {
		(*i)->draw(camera);
	}
	camera.renderSensor();
}

void World::bounceRay(Ray nr, Color c, Obj* src, int bounce)
{
	nr.genLineFormula();
	if (bounce >= bounceLimit) return;
	Point finalCol = Point(), col;
	Vector finalNorm = Vector(), norm;
	double finalPer, per = 0;
	double finalLen = INFINITY;
	Obj* colObj = nullptr;
	for (auto i = objects.begin(); i != objects.end(); i++) {
		if (*i == src) continue;
		if (!(*i)->intersect(nr, &col, &norm, (*i)->behavior == sensor ? &per : 0)) continue;
		double len = Vector(col - nr.o).Length();
		if(len < finalLen){
			finalLen = len;
			finalCol = col;
			finalNorm = norm;
			finalPer = per;
			colObj = *i;
		}
	}

	Vector newVec;
	Ray newRay;

	if (finalCol.isValid()) {
		Line drawLine = Line(nr.o, finalCol);
		camera.drawUnsafe(drawLine, c);

		if (finalNorm.isValid()) {
			switch (colObj->behavior) {
			case reflect:
				newVec = nr.d.Reflect(finalNorm);
				newRay = Ray(finalCol, newVec);
				bounceRay(newRay, c, colObj, ++bounce);
				return;
			case refract:
				newVec = nr.d.Refract(finalNorm, colObj->refInd / nr.curRefIndex);
				newRay = Ray(finalCol, newVec);
				newRay.curRefIndex = colObj->refInd;
				bounceRay(newRay, c, colObj, ++bounce);
				return;
			case sensor:
				camera.pixels[(int)(camera.sensorRes * finalPer)] = camera.pixels[(int)(camera.sensorRes * finalPer)] + c * c.a;
				return;
			case absorb:
			default:
				return;
			}
		}
	}
	else {
		camera.drawRay(nr, c);
	}
}

void World::drawLight(const Point& src, Color c)
{
	for (double i = -lightRange; i < lightRange; i += lightStep) {
		Ray ray = Ray(src, Point(1, i));
		bounceRay(ray, c, nullptr, 0);
	}
}
