#ifndef _DEFS_H_
#define _DEFS_H_

#include <iostream>
#include <cmath>
#include "../SDL2-2.0.14/include/SDL.h"
//http://geomalgorithms.com/a03-_inclusion.html


#define FPS 60
#define _dt (1000/FPS)

#define RELEASE(x) { delete x; x = nullptr; }
#define RELEASE_ARRAY(x) { delete[] x; x = nullptr; }

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

#define GRAVITYEARTH 9.8
#define GRAVITYMOON 1.6

#define pA 1.225

#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

#define G pow(10,-1)*6.6

#define RAD(a) (a*3.14/180)

struct Mouse { int x, y, offsx, offsy, stateL, stateR; };

struct fPoint { float x, y; };

inline void DrawfLine(SDL_Renderer* renderer, fPoint camera, fPoint p0, fPoint p1)
{
	SDL_RenderDrawLine(renderer, p0.x - camera.x, p0.y - camera.y, p1.x - camera.x, p1.y - camera.y);
}

struct fRect { float x, y, w, h; };

inline bool PointInRect(SDL_Point p, SDL_Rect r) { return (p.x > r.x && p.x < r.x + r.w && p.y > r.y && p.y < r.y + r.h); }
inline bool fPointInRect(fPoint p, SDL_Rect r) { return (p.x > r.x && p.x < r.x + r.w && p.y > r.y && p.y < r.y + r.h); }

inline float norm(float x0, float y0, float x1, float y1)
{
	fPoint v{ (x0 - x1),(y0 - y1) };
	return (float(sqrt((v.x * v.x) + (v.y * v.y))));
}
inline float norm(fPoint p0, fPoint p1)
{
	fPoint v{ (p0.x - p1.x),(p0.y - p1.y) };
	return (float(sqrt((v.x * v.x) + (v.y * v.y))));
}
inline float norm(fPoint v) { return (float(sqrt((v.x * v.x) + (v.y * v.y)))); }

struct Timer
{
	Uint32 time;
	Timer() { Start(); };
	void Start() { time = SDL_GetTicks(); };
	/*inline*/Uint32 msRead() { return (SDL_GetTicks() - time); };
	/*inline*/float sRead() { return float((SDL_GetTicks() - time) / 1000.f); };
};

#endif
