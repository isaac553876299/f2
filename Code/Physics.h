#ifndef _PHYSICS_H_
#define _PHYSICS_H_

#include "defs.h"

class Body
{
public:
	fPoint* v = nullptr;
	fPoint center;
	fPoint inert;
	int sides;
	float radius;
	float angle;
	float speed;

	Body(float _x,float _y,int _sides, float _radius, float _angle, float _speed) :
		center{ _x,_y }, sides(_sides), radius(_radius), angle(_angle), speed(_speed)
	{
		v = new fPoint[sides];
		//hmm...
		Reset(_x, _y);
	}
	~Body()
	{
		RELEASE_ARRAY(v);
	}

	void Reset(float _x,float _y)
	{
		center = { _x,_y };
		for (int i = 0; i < sides; i++)
		{
			v[i].x = center.x + radius * cos(RAD((360 / sides) * i));
			v[i].y = center.y + radius * sin(RAD((360 / sides) * i));
		}
		inert = center;

		//angle = 0;
		//speed = 0;
	}

	void Update(float dt)
	{
		center.x += (speed * cos(RAD(angle))) * dt;
		center.y += (speed * sin(RAD(angle))) * dt;
		for (int i = 0; i < sides; i++)
		{
			v[i].x += (speed * cos(RAD(angle))) * dt;
			v[i].y += (speed * sin(RAD(angle))) * dt;
		}
		//inert = center;
	}

	void Draw(SDL_Renderer* renderer)
	{
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		for (int i = 0; i < sides - 1; i++)
			SDL_RenderDrawLine(renderer, v[i].x, v[i].y, v[i + 1].x, v[i + 1].y);
		SDL_RenderDrawLine(renderer, v[sides - 1].x, v[sides - 1].y, v[0].x, v[0].y);

		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		int x2 = center.x + 100 * cos(RAD(angle));
		int y2 = center.y + 100 * sin(RAD(angle));
		SDL_RenderDrawLine(renderer, center.x, center.y, x2, y2);

		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
		SDL_RenderDrawPoint(renderer, inert.x, inert.y);
	}

	void Collision(Body* b2)
	{
		bool collided = false;
		if (this != b2)
		{
			float xd = abs(center.x - b2->center.x);
			float yd = abs(center.y - b2->center.y);
			float distance = sqrt((xd * xd) + (yd * yd));
			if (distance < radius)
			{
				for (int i = 0; i < sides; i++)
				{

				}
			}
		}
	}
};

class Physics
{
public:

	Body* b1 = new Body(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 10, 50, 0, 0);

	Physics()
	{

	}

	~Physics()
	{
		RELEASE(b1);
	}

	void Update(float dt, Mouse _mouse, int* _keyboard)
	{
		//=====FIX=====
		dt = 0.006944f;
		//=====FIX=====

		float angle_i = 100.0f * dt;
		float player_speed_i = 100.0f * dt;
		if (_keyboard[SDL_SCANCODE_LEFT]) b1->angle -= angle_i;
		if (_keyboard[SDL_SCANCODE_RIGHT]) b1->angle += angle_i;
		if (_keyboard[SDL_SCANCODE_UP]) b1->speed += player_speed_i;
		if (_keyboard[SDL_SCANCODE_DOWN]) b1->speed -= player_speed_i;

		if (_keyboard[SDL_SCANCODE_R]) b1->Reset(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

		b1->Update(dt);
	}

	void Draw(SDL_Renderer* renderer)
	{
		b1->Draw(renderer);
	}
};

#endif
