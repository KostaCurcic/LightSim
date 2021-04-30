#include <iostream>
#include "OpenGL3.1.h"
#include "GLInit.h"
#include "World.h"
#include "FunctionalObj.h"

#include "ShaderHelper.h"

#define XRES 900
#define YRES 900


using namespace std;

World world;

//TEMP
Line* bar;
FunctionalObj* fo;
bool moving = false;
bool lensMv = false;
short rWidth = XRES;
short rHeight = YRES;
short mouseXOld;
short mouseYOld;
double rulX, rulY;

double lens(double per) {
	per += 2.5;
	double r = sqrt(6 * per - per * per) - 2.95;
	//r = fmod(r, 0.2);
	return r;
}

void initial(WPARAM wParam, LPARAM lParam) {
	world.camera.aspect = XRES / (float)YRES;
	glOrtho(-world.camera.aspect, world.camera.aspect, -1.0f, 1.0f, -1.0f, 1.0f);
	bar = new Line[2]{ Line(Point(-.5, 0), Point(-.5, .5)), Line(Point(-.5, -0.5), Point(-.5, -1)) };
	bar[0].behavior = absorb;
	bar[1].behavior = absorb;
	world.objects.push_back(bar);
	world.objects.push_back(bar + 1);
	fo = new FunctionalObj[2]{ FunctionalObj(Point(-.5, 0), Point(-.5, -.1), [](double per)
		{
			return lens(per);
		}
	), FunctionalObj(Point(-.5, 0), Point(-.5, -.1), [](double per)
		{
			return -lens(per);
		}
	) };
	fo[0].behavior = refract;
	fo[0].refInd = 1;
	fo[1].behavior = refract;
	fo[1].refInd = 1.5;
	world.objects.push_back(fo);
	world.objects.push_back(fo + 1);

	Line* l = new Line(Point(-1.01, 0.001), Point(-1.01, -0.001));
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
	short x = (short)(lParam & 0xFFFF);
	short y = (short)((lParam >> 16) & 0xFFFF);

	double mx = x / ((double)rWidth) * 2 - 1;
	double my = (YRES - y) / ((double)rHeight) * 2 - 1;

	mx = (mx * (world.camera.rScale * world.camera.aspect)) + world.camera.xPos;
	my = (my * world.camera.rScale) + world.camera.yPos;

	double rulLen = Vector(Point(rulX, rulY) - Point(mx, my)).Length();
	int mod = 0;
	while (rulLen < 1 && mod < 2) {
		mod++;
		rulLen *= 1000;
	}
	cout << '\r' << rulLen << ((mod == 0) ? "" : ((mod == 1) ? "m" : "micro")) << "       ";

	if (moving) {
		double oldX = mouseXOld / ((double)rWidth) * 2 - 1;
		double oldY = (YRES - mouseYOld) / ((double)rHeight) * 2 - 1;
		oldX = (oldX * (world.camera.rScale * world.camera.aspect)) + world.camera.xPos;
		oldY = (oldY * world.camera.rScale) + world.camera.yPos;

		world.camera.xPos -= mx - oldX;
		world.camera.yPos -= my - oldY;
		world.camera.rescaleWorld();

		mouseXOld = x;
		mouseYOld = y;

	}
	else if (lensMv) {

		fo[0].o = Point(mx, my);
		fo[1].o = Point(mx + 0.00, my);
		bar[0].o = Point(mx, my - 0.01);
		bar[1].o = Point(mx, my - 0.49);
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
	switch (wParam)
	{
	case VK_SPACE:
		lensMv = !lensMv;
		break;
	default:
		break;
	}
}

void resize(WPARAM wParam, LPARAM lParam) {
	short xSize = (short)(lParam & 0xFFFF);
	short ySize = (short)((lParam >> 16) & 0xFFFF);
	world.camera.aspect = xSize / (float)ySize;
	world.camera.rescaleWorld();
	glViewport(0, 0, xSize, ySize);
	rWidth = xSize;
	rHeight = ySize;
}

void wheel(WPARAM wParam, LPARAM lParam) {
	short scrolled = (short)((wParam >> 16) & 0xFFFF);
	double mul = pow(1.1, -scrolled / 120);
	world.camera.rScale *= mul;
	world.camera.rescaleWorld();
}

void lmbD(WPARAM wParam, LPARAM lParam) {
	short x = (short)(lParam & 0xFFFF);
	short y = (short)((lParam >> 16) & 0xFFFF);

	mouseXOld = x;
	mouseYOld = y;

	moving = true;
}

void lmbU(WPARAM wParam, LPARAM lParam) {
	moving = false;
}
void rmbD(WPARAM wParam, LPARAM lParam) {
	short x = (short)(lParam & 0xFFFF);
	short y = (short)((lParam >> 16) & 0xFFFF);
	double mx = x / ((double)rWidth) * 2 - 1;
	double my = (YRES - y) / ((double)rHeight) * 2 - 1;

	mx = (mx * (world.camera.rScale * world.camera.aspect)) + world.camera.xPos;
	my = (my * world.camera.rScale) + world.camera.yPos;

	rulX = mx;
	rulY = my;
}

int main() {
	EVENTFUNC fun[] = {
		EVENTFUNC {WM_PAINT, draw},
		EVENTFUNC {WM_SIZE, resize},
		EVENTFUNC {WM_CREATE, initial},
		EVENTFUNC {WM_MOUSEMOVE, mouse},
		EVENTFUNC {WM_KEYUP, keyPress},
		EVENTFUNC {WM_MOUSEWHEEL, wheel},
		EVENTFUNC {WM_LBUTTONDOWN, lmbD},
		EVENTFUNC {WM_LBUTTONUP, lmbU},
		EVENTFUNC {WM_RBUTTONDOWN, rmbD}
	};

	DoGL(XRES, YRES, 9, fun);
}