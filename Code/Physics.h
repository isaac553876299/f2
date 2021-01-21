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

	float directionAngle;
	float rotationAngle;
	fPoint velocity;
	fPoint acceleration;
	fPoint force;
	float mass;
	fPoint gravity;

	Body(int n, float x, float y, float r, float da, float ra, float m)
	{
		nsides = n;
		center = { x,y };
		radius = r;
		vertex = new fPoint[nsides];
		UpdateVertex();
		directionAngle = da;
		rotationAngle = ra;
		mass = m;

	}
	~Body() {};

	void UpdateVertex()
	{
		for (int i = 0; i < nsides; i++)
		{
			vertex[i].x = center.x + radius * cos(RAD((360 / nsides) * i));
			vertex[i].y = center.y + radius * sin(RAD((360 / nsides) * i));
		}
	}

	void Draw(SDL_Renderer* renderer, fPoint camera)
	{
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		for (int i = 0; i < nsides - 1; i++)
			DrawfLine(renderer, camera, vertex[i], vertex[i + 1]);
		DrawfLine(renderer, camera, vertex[nsides - 1], vertex[0]);

		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		fPoint p2{ center.x + 100 * cos(RAD(directionAngle)),center.y + 100 * sin(RAD(directionAngle)) };
		DrawfLine(renderer, camera, center, p2);
	}
};

class Physics
{
public:
	Body* rocket = nullptr;
	Body* earth = nullptr;

	fPoint camera{ 0.f,0.f };

	Physics();
	~Physics();

	void Input(Mouse _mouse, int* _keyboard);

	void Update(float dt);

	void Draw(SDL_Renderer* renderer);

	void Collide(Body* b0, Body* b1);

	void OnCollision();
	void secondLaw();
	void gravity();
};

#endif