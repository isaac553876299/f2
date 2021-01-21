#ifndef _PHYSICS_H_
#define _PHYSICS_H_

#include "defs.h"

class Body
{
public:
	int nsides;
	fPoint center;
	float radius;
	fPoint* vertex;

	fPoint velocity;
	float directionAngle;
	float rotationAngle;

	Body(int n, float x, float y, float r, float vx, float vy, float da, float ra);
	~Body();

	void Draw(SDL_Renderer* renderer);

};

class Physics
{
public:

	Body* b1 = new Body(4, 100.f, 100.f, 50.f, 0.f, 0.f, 0.f, 0.f);

	Physics();
	~Physics();

	void Input(Mouse _mouse, int* _keyboard);
	void Update(float dt);
	void Draw(SDL_Renderer* renderer);

};

#endif
