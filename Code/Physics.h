#ifndef _PHYSICS_H_
#define _PHYSICS_H_

#include "defs.h"

struct spaceship { Rect rect; float x, y, angle, speed; };

class Physics
{
public:

	spaceship player{ {100,100,100,100},100,100,0,0 };

	Physics()
	{

	}

	~Physics()
	{

	}

	void Update(float dt, Mouse _mouse, int* _keyboard)
	{
		//=====FIX=====
		dt = 0.006944f;
		//=====FIX=====

		float angle_i = 100.0f * dt;
		float player_speed_i = 100.0f * dt;
		if (_keyboard[SDL_SCANCODE_LEFT]) player.angle -= angle_i;
		if (_keyboard[SDL_SCANCODE_RIGHT]) player.angle += angle_i;
		if (_keyboard[SDL_SCANCODE_UP]) player.speed += player_speed_i;
		if (_keyboard[SDL_SCANCODE_DOWN]) player.speed -= player_speed_i;

		if (_keyboard[SDL_SCANCODE_R])
		{
			player.x = 100;
			player.y = 100;
			//player.angle = 0;
			//player.speed = 0;
		}


		player.x += (player.speed * cos(RAD(player.angle))) * dt;
		player.y += (player.speed * sin(RAD(player.angle))) * dt;

		player.rect.a = player.x;
		player.rect.b = player.y;
	}

	void Draw(SDL_Renderer* renderer)
	{
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_Rect rect{ player.rect.a ,player.rect.b ,player.rect.c ,player.rect.d };
		SDL_RenderDrawRect(renderer, &rect);

		int x1 = player.rect.a + player.rect.c / 2;
		int y1 = player.rect.b + player.rect.d / 2;

		int x2 = x1 + 100 * cos(RAD(player.angle));
		int y2 = y1 + 100 * sin(RAD(player.angle));

		SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
	}
};

#endif
