#pragma once
#include <Windows.h>

typedef struct EventFunc {
	UINT eventNum;
	void (*func)(WPARAM, LPARAM);
} EVENTFUNC;

void DoGL(int x, int y, int count, EVENTFUNC fcns[]);
