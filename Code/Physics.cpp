#include "Physics.h"

Physics::Physics()
{
	rocket = new Body(4, PIXEL_TO_METERS(18000.f), PIXEL_TO_METERS(25000.f), PIXEL_TO_METERS(2500.0f), 0.f, 0.f, 10000.0f);
	earth = new Body(50, PIXEL_TO_METERS(18000.f), PIXEL_TO_METERS(70000.f), PIXEL_TO_METERS(30000.0f), 0.f, 0.f, 500000.0f);
}
Physics::~Physics() {};

void Physics::Input(Mouse _mouse, int* _keyboard)
{
	if (_keyboard[SDL_SCANCODE_W]) camera.y -= 1.f;
	if (_keyboard[SDL_SCANCODE_S]) camera.y += 1.f;
	if (_keyboard[SDL_SCANCODE_A]) camera.x -= 1.f;
	if (_keyboard[SDL_SCANCODE_D]) camera.x += 1.f;

	if (_keyboard[SDL_SCANCODE_UP]) rocket->velocity.x += 0.1f;
	if (_keyboard[SDL_SCANCODE_DOWN]) rocket->velocity.y -= 0.1f;
	//if (_keyboard[SDL_SCANCODE_3]) rocket->acceleration += 0.1f;
//	if (_keyboard[SDL_SCANCODE_4]) rocket->acceleration -= 0.1f;

	if (_keyboard[SDL_SCANCODE_R]) rocket->center = { 300,300 };
	//if (_keyboard[SDL_SCANCODE_T]) rocket->velocity = 0.f;


	float increment = 1.0f;
	if (_keyboard[SDL_SCANCODE_LEFT]) rocket->directionAngle -= increment;
	if (_keyboard[SDL_SCANCODE_RIGHT]) rocket->directionAngle += increment;
}

void Physics::Update(float dt)//step
{
	gravity();
	rocket->acceleration.x = rocket->force.x / rocket->mass;
	rocket->acceleration.y = rocket->force.y / rocket->mass;

	int i = 0;
	switch (i)
	{
	case 0://Implicit Euler
		rocket->center.x += rocket->velocity.x * dt;
		rocket->center.y += rocket->velocity.y * dt;
		rocket->velocity.x += rocket->acceleration.x * dt;
		rocket->velocity.y += rocket->acceleration.y * dt;
		break;
	case 1://Symplectic Euler
		rocket->velocity.x += rocket->acceleration.x * dt;
		rocket->velocity.y += rocket->acceleration.y * dt;
		rocket->center.x += rocket->velocity.x * dt;
		rocket->center.y += rocket->velocity.y * dt;
		break;
	case 2://Velocity-Verlet
		rocket->center.x += (rocket->velocity.x * dt) + (0.5f * rocket->acceleration.x) * (dt * dt);
		rocket->center.y += (rocket->velocity.y * dt) + (0.5f * rocket->acceleration.y * (dt * dt));
		break;
	case 3://St�rmer-Verlet.

		break;
	default:
		break;
	}

	rocket->UpdateVertex();
}

void Physics::Draw(SDL_Renderer* renderer)
{
	SDL_Rect defaultCamera{ camera.x,camera.y,WINDOW_WIDTH,WINDOW_HEIGHT };
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	SDL_RenderDrawRect(renderer, &defaultCamera);
	rocket->Draw(renderer, camera);
	earth->Draw(renderer, camera);

}

void Physics::Collide(Body* b0, Body* b1)
{

}

void Physics::OnCollision()
{

}
void Physics::secondLaw()
{
	//rocket->force = rocket->mass * rocket->acceleration;
}
void Physics::gravity()
{
	//if (onEarth)
	{
		float r = norm(rocket->center, earth->center);
		rocket->gravity.x = (-G * (((rocket->mass) * (earth->mass)) / (r * r)) * (rocket->center.x - earth->center.x));
		rocket->gravity.y = -G * (((rocket->mass) * (earth->mass)) / (r * r)) * (rocket->center.y - earth->center.y);

		rocket->force = { rocket->gravity.x, rocket->gravity.y };

	}

}

