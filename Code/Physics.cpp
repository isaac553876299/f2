#include "Physics.h"

Physics::Physics()
{
	rocket = new Body(4, PIXEL_TO_METERS(18000.f), PIXEL_TO_METERS(25000.f), PIXEL_TO_METERS(2500.0f), 0.f, 0.f, 10000.0f);//2500
	earth = new Body(50, PIXEL_TO_METERS(18000.f), PIXEL_TO_METERS(70000.f), PIXEL_TO_METERS(30000.0f), 0.f, 0.f, 500000.0f);//30000
	moon = new Body(50, PIXEL_TO_METERS(18000.f), PIXEL_TO_METERS(-1000000.f), PIXEL_TO_METERS(15000.0f), 0.f, 0.f, 50000.0f);
}
Physics::~Physics() {};

void Physics::Input(Mouse _mouse, int* _keyboard)
{
	//KeyStates::IDLE_0,DOWN_1,REPEAT_2,UP_3

	if (_mouse.stateL == 1)
	{
		rocket->center = { float(_mouse.x+camera.x),float(_mouse.y+camera.y) };
	}

	if (_keyboard[SDL_SCANCODE_W]) camera.y -= 10.f;
	if (_keyboard[SDL_SCANCODE_S]) camera.y += 10.f;
	if (_keyboard[SDL_SCANCODE_A]) camera.x -= 10.f;
	if (_keyboard[SDL_SCANCODE_D]) camera.x += 10.f;

	if (_keyboard[SDL_SCANCODE_M])
	{
		rocket->impulseForce.x = 600.0f * cos(RAD(rocket->directionAngle));
		rocket->impulseForce.y = 600.0f * sin(RAD(rocket->directionAngle));
	}
	else
	{
		rocket->impulseForce.x = 0.0f;
		rocket->impulseForce.y = 0.0f;
	}

	if (_keyboard[SDL_SCANCODE_R]) rocket->center = { 300,300 };
	if (_keyboard[SDL_SCANCODE_T]) rocket->velocity = { 0.f,0.f };


	float increment = 3.0f;
	if (_keyboard[SDL_SCANCODE_LEFT]) rocket->directionAngle -= increment;
	if (_keyboard[SDL_SCANCODE_RIGHT]) rocket->directionAngle += increment;
}

void Physics::Update(float dt)//step
{
	gravity();
	motorImpulse(dt);
	calculateForces();

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
	case 3://Störmer-Verlet.

		break;
	default:
		break;
	}

	
	rocket->UpdateVertex();
	Collide(rocket, earth);
	camera.x = rocket->center.x - (WINDOW_WIDTH / 2);
	camera.y = rocket->center.y - (WINDOW_HEIGHT / 2);
}

void Physics::Draw(SDL_Renderer* renderer)
{
	SDL_Rect defaultCamera{ camera.x,camera.y,WINDOW_WIDTH,WINDOW_HEIGHT };
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	DrawfLine(renderer, camera, rocket->center, earth->center);
	DrawfLine(renderer, camera, rocket->center, moon->center);
	rocket->Draw(renderer, camera);
	earth->Draw(renderer, camera);
	moon->Draw(renderer, camera);
}

void Physics::Collide(Body* b0, Body* b1)
{
	//b0 = spaceship
	if (b0 != nullptr && b1 != nullptr)
	{
		float dist = norm(b0->center, b1->center);
		if (dist < (b0->radius + b1->radius))
		{
			rocket->velocity = { 0.f,0.f };
			float vx = (rocket->center.x - earth->center.x) / dist;
			float vy = (rocket->center.y - earth->center.y) / dist;
			rocket->center.x = earth->center.x + (earth->radius + rocket->radius) * cos(acos(vx));
			rocket->center.y = earth->center.y + (earth->radius + rocket->radius) * sin(asin(vy));
		}
	}
}

void Physics::OnCollision()
{
	
}
void Physics::secondLaw()
{
	//rocket->force = rocket->mass * rocket->acceleration;
}
void Physics::gravity()
{	//if (onEarth)
	{
		float r = norm(rocket->center, earth->center);
		float r2 = norm(rocket->center, moon->center);
		rocket->gravity.x = (-G * (((rocket->mass) * (earth->mass)) / (r * r)) * (rocket->center.x - earth->center.x)); -(-G * (((rocket->mass) * (moon->mass)) / (r2 * r2)) * (rocket->center.x - moon->center.x));
		rocket->gravity.y = (-G * (((rocket->mass) * (earth->mass)) / (r * r)) * (rocket->center.y - earth->center.y)); -(-G * (((rocket->mass) * (moon->mass)) / (r2 * r2)) * (rocket->center.y - moon->center.y));
	}
}

void Physics::motorImpulse(float dt)
{
		rocket->impulse.x = rocket->impulseForce.x * rocket->mass;
		rocket->impulse.y = rocket->impulseForce.y * rocket->mass;
}
void Physics::calculateForces()
{
	float xForces;
	float yForces;
	xForces = rocket->gravity.x + rocket->impulse.x;
	yForces = rocket->gravity.y + rocket->impulse.y;

	rocket->force = { xForces,yForces };
}
