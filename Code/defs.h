#ifndef _DEFS_H_
#define _DEFS_H_
#include <iostream>
#include "../SDL2-2.0.14/include/SDL.h"

#define FPS 60
#define _dt (1000/FPS)/1000
//dt_30_60_120_144{0.0333,0.0167,0.0083,0.00694}

#define WW 1280
#define WH 720
#define RAD(a) (a*3.14/180)

#define RELEASE(x) { delete x; x = nullptr; }

struct Mouse { int x, y, offsx, offsy, stateL, stateR; };

struct Point { int a, b; };
struct fPoint { float a, b; };
struct Rect { int a, b, c, d; };

inline bool PointInRect(Point p, Rect r) { return (p.a > r.a && p.a < r.a + r.c && p.b > r.b && p.b < r.b + r.d); }
inline bool fPointInRect(fPoint p, Rect r) { return (p.a > r.a && p.a < r.a + r.c && p.b > r.b && p.b < r.b + r.d); }

struct Timer
{
	Uint32 time;
	Timer() { Start(); };
	void Start() { time = SDL_GetTicks(); };
	inline Uint32 msRead() { return time; };
	inline float sRead() { return float(time / 1000); };
};

#endif
