#ifndef _DEFS_H_
#define _DEFS_H_
#include <iostream>
#include "../SDL2-2.0.14/include/SDL.h"

#define FPS 60
#define WW 1280
#define WH 720
#define RAD(a) (a*3.14/180)

#define RELEASE(x) { delete x; x = nullptr; }

struct mouse { int x, y, offsx, offsy, stateL, stateR; };

struct point { int a, b; };
struct fpoint { float a, b; };
struct rect { int a, b, c, d; };

inline bool PointInRect(point p, rect r) { return (p.a > r.a && p.a < r.a + r.c && p.b > r.b && p.b < r.b + r.d); }
inline bool fPointInRect(fpoint p, rect r) { return (p.a > r.a && p.a < r.a + r.c && p.b > r.b && p.b < r.b + r.d); }

#endif
