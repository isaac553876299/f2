#include <iostream>
#include "../SDL2-2.0.14/include/SDL.h"

#define WW 1280
#define WH 720
#define FPS 1

struct mouse { int x, y, offsx, offsy, stateL, stateR; };

#define RAD(a) (a*3.14/180)

struct point { int a, b; };
struct rect { int a, b, c, d; };

bool PointInRect(point p, rect r)
{
	return (p.a > r.a && p.a < r.a + r.c && p.b > r.b && p.b < r.b + r.d);
}