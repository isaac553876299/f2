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
	bool* cv = nullptr;

	Body(float _x,float _y,int _sides, float _radius, float _angle, float _speed) :
		center{ _x,_y }, sides(_sides), radius(_radius), angle(_angle), speed(_speed)
	{
		v = new fPoint[sides];
		cv = new bool[sides];
		//hmm...
		Reset(_x, _y, false);
	}
	~Body()
	{
		RELEASE_ARRAY(v);
	}

	void Reset(float _x,float _y, bool sOnly)
	{
		if (sOnly)
		{
			speed = 0;
		}
		else
		{
			center = { _x,_y };
			for (int i = 0; i < sides; i++)
			{
				v[i].x = center.x + radius * cos(RAD((360 / sides) * i));
				v[i].y = center.y + radius * sin(RAD((360 / sides) * i));

				cv[i] = false;
			}
			inert = center;
		}
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

		SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
		for (int i = 0; i < sides; i++)
		{
			if (cv[i])
				SDL_RenderDrawPoint(renderer, v[i].x, v[i].y);
		}
	}

	bool Collision(Body* b2)
	{
		if (this != b2)
		{
			float xd = abs(center.x - b2->center.x);
			float yd = abs(center.y - b2->center.y);
			float distance = sqrt((xd * xd) + (yd * yd));
			if (distance < radius) return (wn_PnPoly(b2->v, b2->sides) != 0);
		}
		return false;
	}

	//=================================================
	//http://geomalgorithms.com/a03-_inclusion.htmldark
	//=================================================
	/*inline*/int isLeft(fPoint p0, fPoint p1, fPoint p2)
	{
		return ((p1.x < p0.x) * (p2.y - p0.y) - (p2.x - p0.x) * (p1.y - p0.y));
	}
	int wn_PnPoly(fPoint* v2, int n)
	{
		int wn = 0;
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				if ((v[i].y <= v2[j].y) && (v[i + 1].y > v2[j].y) && (isLeft(v[i], v[i + 1], v2[j]) > 0)) ++wn;
				else { if ((v[i + 1].y <= v2[j].y) && (isLeft(v[i], v[i + 1], v2[j]) < 0)) --wn; }
			}
		}
		return wn;
	}
};

class Physics
{
public:

	Body* b1 = new Body(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 7, 100, 0, 0);
	Body* b2 = new Body(800, 300, 5, 50, 0, 0);
	Body* b3 = new Body(800, 400, 4, 50, 0, 0);

	int a = 72;
	int b = a / 2;
	Body* example[9] =
	{
		new Body(a, a * 1, 3, b, 0, 0),
		new Body(a, a * 2, 4, b, 0, 0),
		new Body(a, a * 3, 5, b, 0, 0),
		new Body(a, a * 4, 6, b, 0, 0),
		new Body(a, a * 5, 7, b, 0, 0),
		new Body(a, a * 6, 8, b, 0, 0),
		new Body(a, a * 7, 9, b, 0, 0),
		new Body(a, a * 8, 10, b, 0, 0),
		new Body(a, a * 9, 360, b, 0, 0),
	};

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
		if (_keyboard[SDL_SCANCODE_UP]) b1->speed += player_speed_i;
		if (_keyboard[SDL_SCANCODE_DOWN]) b1->speed -= player_speed_i;

		if (_keyboard[SDL_SCANCODE_R]) b1->Reset(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, false);
		if (_keyboard[SDL_SCANCODE_T]) b1->Reset(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, true);

		b1->Update(dt);

		collision[0] = b1->Collision(b2);
		collision[1] = b1->Collision(b3);
	}

	void Draw(SDL_Renderer* renderer)
	{
		b1->Draw(renderer);
		b2->Draw(renderer);
		b3->Draw(renderer);


		for (int i = 0; i < 9; i++)
			example[i]->Draw(renderer);
	}
};

#endif
