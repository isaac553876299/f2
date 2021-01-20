#include "Physics.h"
//=============================================
//http://geomalgorithms.com/a03-_inclusion.html
//=============================================

#include "defs.h"

void Body::Draw(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	for (int i = 0; i < sizeof(vertex) - 1; i++)
		DrawfLine(renderer, vertex[i], vertex[i + 1]);
	DrawfLine(renderer, vertex[sizeof(vertex) - 1], vertex[0]);

	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	fPoint p2{ center.x + 100 * cos(RAD(angle)),center.y + 100 * sin(RAD(angle)) };
	DrawfLine(renderer, center, p2);
}

Physics::Physics()
{

}

Physics::~Physics()
{

}

void Physics::Update(float dt, Mouse _mouse, int* _keyboard)
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

void Physics::Draw(SDL_Renderer* renderer)
{
	b1->Draw(renderer);
	b2->Draw(renderer);
}

