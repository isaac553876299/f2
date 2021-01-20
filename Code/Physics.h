#ifndef _PHYSICS_H_
#define _PHYSICS_H_

//=============================================
//http://geomalgorithms.com/a03-_inclusion.html
//=============================================

#include "defs.h"

struct wbound
{
	fPoint v;
	bool in;
};

class Body
{
public:
	fPoint* v = nullptr;
	fPoint center;
	int sides;
	float radius;
	float angle;
	float rotation;
	float speed;
	wbound* b = nullptr;

	Body(float _x,float _y,int _sides, float _radius, float _angle, float _rotation, float _speed) :
		center{ _x,_y }, sides(_sides), radius(_radius), angle(_angle), rotation(_rotation), speed(_speed)
	{
		v = new fPoint[sides];
		b = new wbound[360];
		ResetPos(_x, _y);
	}
	~Body()
	{
		RELEASE_ARRAY(v);
	}

	void ResetPos(float _x,float _y)
	{
		center = { _x,_y };
		for (int i = 0; i < sides; i++)
		{
			v[i].x = center.x + radius * cos(RAD((360 / sides) * i));//+rotation
			v[i].y = center.y + radius * sin(RAD((360 / sides) * i));//+rotation
		}
		for (int i = 0; i < 360; i++)
		{
			b[i].v.x = center.x + radius * cos(RAD(i));
			b[i].v.y = center.y + radius * sin(RAD(i));
			b[i].in = false;
		}
	}

	void Update(float dt)
	{
		center.x += (speed * cos(RAD(angle))) * dt;
		center.y += (speed * sin(RAD(angle))) * dt;
		ResetPos(center.x, center.y);
	}

	void Draw(SDL_Renderer* renderer)
	{
		//draw *v
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		for (int i = 0; i < sides - 1; i++)
			SDL_RenderDrawLine(renderer, v[i].x, v[i].y, v[i + 1].x, v[i + 1].y);
		SDL_RenderDrawLine(renderer, v[sides - 1].x, v[sides - 1].y, v[0].x, v[0].y);

		//draw *b
		for (int i = 0; i < 359; i++)
		{
			if (b[i].in == false) SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
			else SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
			SDL_RenderDrawLine(renderer, b[i].v.x, b[i].v.y, b[i + 1].v.x, b[i + 1].v.y);
		}
		if (b[359].in == false) SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
		else SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
		SDL_RenderDrawLine(renderer, b[359].v.x, b[359].v.y, b[0].v.x, b[0].v.y);

		//draw radius
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		int x2 = center.x + 100 * cos(RAD(angle));
		int y2 = center.y + 100 * sin(RAD(angle));
		SDL_RenderDrawLine(renderer, center.x, center.y, x2, y2);
	}

	float mod(fPoint p0, fPoint p1)
	{
		fPoint cd{ (abs(p0.x - p1.x)),(abs(p0.y - p1.y)) };
		return (float(sqrt((cd.x * cd.x) + (cd.y * cd.y))));
	}
	void Collision(Body* b2)
	{
		if (this != b2)
			if (mod(center, b2->center) < (radius + b2->radius))
				for (int i = 0; i < 360; i++)
					if (mod(b[i].v, b2->center) < b2->radius)
						b[i].in = true;
	}
};

class Physics
{
public:

	Body* b1 = new Body(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 7, 100, 0, 0, 0);
	Body* b2 = new Body(800, 300, 3, 50, 0, 0, 0);

	bool collision[2] = { false,false };

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

		if (_keyboard[SDL_SCANCODE_Z]) b1->rotation -= angle_i;
		if (_keyboard[SDL_SCANCODE_X]) b1->rotation += angle_i;

		if (_keyboard[SDL_SCANCODE_UP]) b1->speed += player_speed_i;
		if (_keyboard[SDL_SCANCODE_DOWN]) b1->speed -= player_speed_i;

		if (_keyboard[SDL_SCANCODE_R]) b1->ResetPos(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
		if (_keyboard[SDL_SCANCODE_T]) b1->speed = 0;

		b1->Update(dt);

		b1->Collision(b2);
	}

	void Draw(SDL_Renderer* renderer)
	{
		b1->Draw(renderer);
		b2->Draw(renderer);
	}
};

#endif
