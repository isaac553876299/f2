#include "Physics.h"
//=============================================
//http://geomalgorithms.com/a03-_inclusion.html
//=============================================

#include "defs.h"

Body::Body(int n, float x, float y, float r, float vx, float vy, float da, float ra)
{
	nsides = n;
	center = { x,y };
	radius = r;
	vertex = new fPoint[nsides];
	for (int i = 0; i < nsides; i++)
	{
		vertex[i].x = center.x + radius * cos(RAD((360 / nsides) * i));
		vertex[i].y = center.y + radius * sin(RAD((360 / nsides) * i));
	}
}
Body::~Body()
{

}

void Body::Draw(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	for (int i = 0; i < sizeof(vertex) - 1; i++)
		DrawfLine(renderer, vertex[i], vertex[i + 1]);
	DrawfLine(renderer, vertex[sizeof(vertex) - 1], vertex[0]);

	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	fPoint p2{ center.x + 100 * cos(RAD(directionAngle)),center.y + 100 * sin(RAD(directionAngle)) };
	DrawfLine(renderer, center, p2);
}

Physics::Physics()
{

}

Physics::~Physics()
{

}

void Physics::Input(Mouse _mouse, int* _keyboard)
{
	float increment = 0.1f;

	if (_keyboard[SDL_SCANCODE_LEFT]) b1->directionAngle -= increment;
	if (_keyboard[SDL_SCANCODE_RIGHT]) b1->directionAngle += increment;

	if (_keyboard[SDL_SCANCODE_Z]) b1->rotationAngle -= increment;
	if (_keyboard[SDL_SCANCODE_X]) b1->rotationAngle += increment;

	if (_keyboard[SDL_SCANCODE_UP])
	{
		b1->velocity.x += increment * cos(b1->directionAngle);
		b1->velocity.y += increment * sin(b1->directionAngle);
	}
	if (_keyboard[SDL_SCANCODE_DOWN])
	{
		b1->velocity.x -= increment * cos(b1->directionAngle);
		b1->velocity.y -= increment * sin(b1->directionAngle);
	}

	if (_keyboard[SDL_SCANCODE_R]) b1->center = { WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 };
	if (_keyboard[SDL_SCANCODE_T]) b1->velocity = { 0.f,0.f };
}

void Physics::Update(float dt)
{
	//=====FIX=====
	dt = 0.006944f;
	//=====FIX=====


}

void Physics::Draw(SDL_Renderer* renderer)
{
	b1->Draw(renderer);

}

