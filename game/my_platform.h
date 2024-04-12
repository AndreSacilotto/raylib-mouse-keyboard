#pragma once
#ifndef __MY_PLATFORM_H__
#define __MY_PLATFORM_H__

typedef struct Vector2I {
	int x;
	int y;
} Vector2I;

Vector2I GetMouseMonitorPosition(void);

bool GetMouseKeyLeft();

bool GetMouseKeyRight();

bool GetMouseKeyMiddle();

bool GetMouseWhell();
#endif