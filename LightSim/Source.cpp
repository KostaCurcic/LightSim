#include <iostream>
#include "OpenGL3.1.h"
#include "GLInit.h"
#include "World.h"

#include "ShaderHelper.h"

#define XRES 1600
#define YRES 900


using namespace std;

World world;

//TEMP
Line* bar;

void initial(WPARAM wParam, LPARAM lParam) {
	bar = new Line[2]{ Line(Point(0, 0.5), Point(0.2, -0.5)), Line(Point(0.5, 0.5), Point(0.2, -0.5)) };
	bar[0].behavior = refract;
	bar[0].refInd = 2.5;
	bar[1].behavior = refract;
	bar[1].refInd = 1;
	world.objects.push_back(bar);
	world.objects.push_back(bar + 1);
}

void mouse(WPARAM wParam, LPARAM lParam) {
	double mx = ((short)(lParam & 0xFFFF)) / ((double)XRES) * 2 - 1;
	double my = (YRES - (short)((lParam >> 16) & 0xFFFF)) / ((double)YRES) * 2 - 1;
	bar[0].o = Point(mx, my);
	bar[1].o = Point(mx - 0.1, my);
}

void draw(WPARAM wParam, LPARAM lParam) {
	glClearColor(1, 1, 1, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_LINE_SMOOTH);

	world.draw();
}

int main() {
	EVENTFUNC fun[] = {
		EVENTFUNC {WM_PAINT, draw},
		EVENTFUNC {WM_CREATE, initial},
		EVENTFUNC {WM_MOUSEMOVE, mouse},
		//EVENTFUNC {WM_KEYUP, keyPress}
	};

	DoGL(1600, 900, 3, fun);
}