#ifndef _PHYSICS_H_
#define _PHYSICS_H_

#include "defs.h"

struct Body
{
	fPoint center;
	float radius;
	fPoint* vertex;

	fPoint velocity;
	float directionAngle;
	float rotationAngle;

	Body(int n, float x, float y, float r, fPoint* v, float vx, float vy, float da, float ra)
	{
		center = { x,y };
		radius = r;
		vertex = new fPoint[n];
		for (int i = 0; i < n; i++)
		{
			vertex[i].x = center.x + radius * cos(RAD((360 / n) * i));
			vertex[i].y = center.y + radius * sin(RAD((360 / n) * i));
		}
	}
	~Body();
};

class Physics
{
public:

	Body* b1 = new Body(100, 100, 100, 4, 0, 0, 0, 0);

	Physics();
	~Physics();

	void Update(float dt, Mouse _mouse, int* _keyboard);
	void Draw(SDL_Renderer* renderer);
};

#endif
