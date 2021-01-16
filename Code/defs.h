#ifndef _DEFS_H_
#define _DEFS_H_
#include <iostream>
#include "../SDL2-2.0.14/include/SDL.h"

#define FPS 60
#define _dt (1000/FPS)/1000
//dt_30_60_120_144{0.0333,0.0167,0.0083,0.00694}

#define RELEASE(x) { delete x; x = nullptr; }
#define RELEASE_ARRAY(x) { delete[] x; x = nullptr; }

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

#define RAD(a) (a*3.14/180)

struct Mouse { int x, y, offsx, offsy, stateL, stateR; };

struct fPoint { float x, y; };

bool PointInRect(SDL_Point p, SDL_Rect r) { return (p.x > r.x && p.x < r.x + r.w && p.y > r.y && p.y < r.y + r.h); }
bool fPointInRect(fPoint p, SDL_Rect r) { return (p.x > r.x && p.x < r.x + r.w && p.y > r.y && p.y < r.y + r.h); }

struct Timer
{
	Uint32 time;
	Timer() { Start(); };
	void Start() { time = SDL_GetTicks(); };
	/*inline¿*/Uint32 msRead() { return time; };
	/*inline¿*/float sRead() { return float(time / 1000); };
};

#endif
