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
	fPoint* circle;

	float directionAngle;
	float rotationAngle;
	fPoint velocity;
	fPoint acceleration;
	fPoint force;
	fPoint dragForce;
	float mass;
	fPoint gravity;
	fPoint impulse;
	fPoint impulseForce = {0,0};
	float xForces;
	float yForces;
	fPoint buo;

	Body(int n, float x, float y, float r, float da, float ra, float m)
	{
		nsides = n;
		center = { x,y };
		radius = r;
		circle = new fPoint[360];
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
		for (int i = 0; i < 360; i++)
		{
			circle[i].x = center.x + radius * cos(RAD(i));
			circle[i].y = center.y + radius * sin(RAD(i));
		}
	}

	void Draw(SDL_Renderer* renderer, fPoint camera, bool debugCollisions)
	{
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		for (int i = 0; i < nsides - 1; i++)
			DrawfLine(renderer, camera, vertex[i], vertex[i + 1]);
		DrawfLine(renderer, camera, vertex[nsides - 1], vertex[0]);

		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		fPoint p2{ center.x + 100 * cos(RAD(directionAngle)),center.y + 100 * sin(RAD(directionAngle)) };
		DrawfLine(renderer, camera, center, p2);

		SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
		if (debugCollisions)
		{
			for (int i = 0; i < 359; i++)
			{
				DrawfLine(renderer, camera, circle[i], circle[i + 1]);
			}
			DrawfLine(renderer, camera, circle[359], circle[0]);
		}
	}
};
class Physics
{
public:
	Body* rocket = nullptr;
	Body* atmos = nullptr;
	Body* moonCrash = nullptr;

	Body* planets[5];

	fPoint camera{ 0.f,0.f };

	bool spaced = false;
	bool debugCollisions = false;

	Physics();
	~Physics();

	void Input(Mouse _mouse, int* _keyboard);

	void Update(float dt);

	void Draw(SDL_Renderer* renderer, SDL_Texture* textures[10]);

	void Collide(Body* b0, Body* b1);

	void OnCollision();
	void secondLaw();
	void gravity();
	void thirdLaw();
	void calculateForces();
	void motorImpulse(float dt);
	void kinematicEnergy();
	void potentialEnergy();
	void aeroDrag();
	void buoyancy();
	void checkMoonColision();
	SDL_Rect posRect;

	bool moonCrashed = false;

	bool moonLanded = false;
	bool crashed = false;
	bool landed = false;
};
#endif