#include <iostream>
#include "OpenGL3.1.h"
#include "GLInit.h"
#include "World.h"
#include "FunctionalObj.h"

#include "ShaderHelper.h"

#define XRES 1600
#define YRES 900


using namespace std;

World world;

//TEMP
Line* bar;
FunctionalObj* fo;
bool moving = false;

double lens(double per) {
	per = per * 2 - 1;
	double r = 1 - (per * per);
	return r / 25;
}

void initial(WPARAM wParam, LPARAM lParam) {
	glOrtho(-world.camera.aspect, world.camera.aspect, -1.0f, 1.0f, -1.0f, 1.0f);
	bar = new Line[2]{ Line(Point(-.5, 0), Point(-.5, .5)), Line(Point(-.5, -0.5), Point(-.5, -1)) };
	bar[0].behavior = absorb;
	bar[1].behavior = absorb;
	world.objects.push_back(bar);
	world.objects.push_back(bar + 1);
	fo = new FunctionalObj[2]{ FunctionalObj(Point(-.5, 0), Point(-.5, -.5), [](double per)
		{
			return lens(per);
		}
	), FunctionalObj(Point(-.5, 0), Point(-.5, -.5), [](double per)
		{
			return -lens(per);
		}
	) };
	fo[0].behavior = refract;
	fo[0].refInd = 1;
	fo[1].behavior = refract;
	fo[1].refInd = 2;
	world.objects.push_back(fo);
	world.objects.push_back(fo + 1);

	Line* l = new Line(Point(-1, 0.5), Point(-1, -0.5));
	l->behavior = sensor;
	world.objects.push_back(l);


	/*fo = new FunctionalObj[1]{ FunctionalObj(Point(-.5, .5), Point(-.5, -.5), [](double per)
		{
			return lens(per);
		}
	) };
	fo->behavior = reflect;
	world.objects.push_back(fo);

	Line* l = new Line(Point(0.75, 0.1), Point(0.75, -0.1));
	l->behavior = sensor;
	world.objects.push_back(l);*/
}

void mouse(WPARAM wParam, LPARAM lParam) {
	double mx = ((short)(lParam & 0xFFFF)) / ((double)XRES) * 2 - 1;
	double my = (YRES - (short)((lParam >> 16) & 0xFFFF)) / ((double)YRES) * 2 - 1;
	if (moving) {
		fo[0].o = Point(mx * world.camera.aspect, my);
		fo[1].o = Point(mx * world.camera.aspect + 0.00, my);
		bar[0].o = Point(mx * world.camera.aspect, my - 0.01);
		bar[1].o = Point(mx * world.camera.aspect, my - 0.49);
	}
}

void draw(WPARAM wParam, LPARAM lParam) {
	glClearColor(1, 1, 1, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_LINE_SMOOTH);

	world.draw();
}

void keyPress(WPARAM wParam, LPARAM lParam) {
	if (wParam == VK_SPACE) {
		moving = !moving;
	}
}

int main() {
	EVENTFUNC fun[] = {
		EVENTFUNC {WM_PAINT, draw},
		EVENTFUNC {WM_CREATE, initial},
		EVENTFUNC {WM_MOUSEMOVE, mouse},
		EVENTFUNC {WM_KEYUP, keyPress}
	};

	DoGL(1600, 900, 4, fun);
}