#include "Physics.h"

Physics::Physics()
{
	rocket = new Body(7, PIXEL_TO_METERS(18000.f), PIXEL_TO_METERS(25000.f), PIXEL_TO_METERS(2500.0f), 0.f, 0.f, 20000.0f);
	//earth
	planets[0] = new Body(50, PIXEL_TO_METERS(18000.f), PIXEL_TO_METERS(70000.f), PIXEL_TO_METERS(30000.0f), 0.f, 0.f, 500000.0f);
	//moon
	planets[1] = new Body(50, PIXEL_TO_METERS(18000.f), PIXEL_TO_METERS(-180000.f), PIXEL_TO_METERS(15000.0f), 0.f, 0.f, 300000.0f);
	//fluid
	planets[2] = new Body(25, PIXEL_TO_METERS(18000.f), PIXEL_TO_METERS(10000), PIXEL_TO_METERS(10000.0f), 0.f, 0.f, 10000.0f);
	//atmosphere
	atmos = new Body(50, PIXEL_TO_METERS(18000.f), PIXEL_TO_METERS(70000.f), PIXEL_TO_METERS(50000.0f), 0.f, 0.f, 0.0f);
	//moon colision and crash
	moonCrash = new Body(50, PIXEL_TO_METERS(18000.f), PIXEL_TO_METERS(-180000.f), PIXEL_TO_METERS(18000.0f), 0.f, 0.f, 0.0f);
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

	spaced = _keyboard[SDL_SCANCODE_SPACE];
	if (_keyboard[SDL_SCANCODE_SPACE])
	{
		rocket->impulseForce.x = 600.0f * cos(RAD(rocket->directionAngle));
		rocket->impulseForce.y = 600.0f * sin(RAD(rocket->directionAngle));
	}
	else if (_keyboard[SDL_SCANCODE_L])
	{
		rocket->impulseForce.x = -800.0f * cos(RAD(rocket->directionAngle));
		rocket->impulseForce.y = -800.0f * sin(RAD(rocket->directionAngle));
	}
	else
	{
		rocket->impulseForce.x = 0;
		rocket->impulseForce.y = 0;
	}
	if (rocket->velocity.y >= 700)
	{
		rocket->velocity.y = 700;
	}
	if (rocket->velocity.y <= -700)
	{
		rocket->velocity.y = -700;
	}
	if (rocket->velocity.x >= 700)
	{
		rocket->velocity.x = 700;
	}
	if (rocket->velocity.x <= -700)
	{
		rocket->velocity.x = -700;
	}
	if (_keyboard[SDL_SCANCODE_R]) rocket->center = { 300,300 };
	if (_keyboard[SDL_SCANCODE_T]) rocket->velocity = { 0.f,0.f };


	float increment = 3.0f;
	if (_keyboard[SDL_SCANCODE_LEFT]) rocket->directionAngle -= increment;
	if (_keyboard[SDL_SCANCODE_RIGHT]) rocket->directionAngle += increment;

	if (_keyboard[SDL_SCANCODE_D] == 3) debugCollisions = !debugCollisions;
}

void Physics::Update(float dt)//step
{
	gravity();
	motorImpulse(dt);
	aeroDrag();
	calculateForces();
	buoyancy();
	checkMoonColision();

	rocket->acceleration.x = rocket->force.x / rocket->mass;
	rocket->acceleration.y = rocket->force.y / rocket->mass;
	
	int i = 0;
	switch (i)
	{
	case 0://Implicit Euler
		rocket->center.x += (rocket->velocity.x * dt);
		rocket->center.y += (rocket->velocity.y * dt);
		rocket->velocity.x += rocket->acceleration.x * dt;
		rocket->velocity.y += rocket->acceleration.y * dt;
		break;
	case 1://Symplectic Euler
		rocket->velocity.x += rocket->acceleration.x * dt;
		rocket->velocity.y += rocket->acceleration.y * dt;
		rocket->center.x += (rocket->velocity.x * dt);
		rocket->center.y += (rocket->velocity.y * dt);
		break;
	case 2://Velocity-Verlet
		rocket->center.x += ((rocket->velocity.x * dt) + (0.5f * rocket->acceleration.x) * (dt * dt));
		rocket->center.y += ((rocket->velocity.y * dt) + (0.5f * rocket->acceleration.y * (dt * dt)));
		break;
	case 3://Störmer-Verlet.

		break;
	default:
		break;
	}

	
	rocket->UpdateVertex();
	for (int i = 0; i < 5; i++)
	{
		if (planets[i] != nullptr && i!=2)
		{
			Collide(rocket, planets[i]);
		}
			
	}
	camera.x = rocket->center.x - (WINDOW_WIDTH / 2);
	camera.y = rocket->center.y - (WINDOW_HEIGHT / 2);
}

void Physics::Draw(SDL_Renderer* renderer, SDL_Texture* textures[10])
{
	rocket->Draw(renderer, camera, debugCollisions);
	for (int i = 0; i < 5; i++)
	{
		if (planets[i] != nullptr)
		{
			planets[i]->Draw(renderer, camera, debugCollisions);
			SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
			DrawfLine(renderer, camera, rocket->center, planets[i]->center);
		}
	}

	atmos->Draw(renderer, camera, debugCollisions);
	moonCrash->Draw(renderer, camera, debugCollisions);

	//sidebar
	SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
	float prop = WINDOW_HEIGHT * abs(planets[1]->center.y - rocket->center.y) / abs(planets[1]->center.y - planets[0]->center.y);
	SDL_Rect pos1 = { 50,prop,10,10 };
	SDL_RenderFillRect(renderer, &pos1);
	SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
	SDL_Rect pos2 = { 50,50,10,10 };
	SDL_RenderFillRect(renderer, &pos2);
	SDL_Rect pos3 = { 50,WINDOW_HEIGHT - 50,10,10 };
	SDL_RenderFillRect(renderer, &pos3);

	SDL_Rect tex = { (rocket->center.x - camera.x - rocket->radius),(rocket->center.y - camera.y - rocket->radius), 2 * rocket->radius, 2 * rocket->radius };
	SDL_RenderCopy(renderer, textures[0], 0, &tex);


	SDL_RenderPresent(renderer);
}

void Physics::Collide(Body* b0, Body* b1)
{
	//b0 = spaceship
	for (int i = 0; i < 5; i++)
	{
		//b0 = rocket;
		//b1 = planets[i];
		if (b0 != nullptr && b1 != nullptr)
		{
			float dist = norm(b0->center, b1->center);
			if (dist < (b0->radius + b1->radius))
			{
				if (!moonLanded && !spaced)
				{
					rocket->velocity = { 0.f,0.f };
				}
				float vx = (rocket->center.x - b1->center.x) / dist;
				float vy = (rocket->center.y - b1->center.y) / dist;
				rocket->center.x = (b1->center.x + (b1->radius + rocket->radius) * cos(acos(vx)));
				rocket->center.y = (b1->center.y + (b1->radius + rocket->radius) * sin(asin(vy)));

				if (b1 == planets[1])
				{
					moonLanded = true;
				}
				else
				{
					moonLanded = false;
				}
				if (moonLanded && !spaced)
				{
					rocket->velocity = { 0.f,0.f };
				}
			}
		}
	}
}

void Physics::OnCollision()
{
	
}
void Physics::secondLaw()
{

}
void Physics::gravity()
{
	fPoint grav = { 0.f,0.f };
	for (int i = 0; i < 5; i++)
	{
		if (planets[i] != nullptr)
		{
			float r = norm(rocket->center, planets[i]->center);
			grav.x += (-G * (((rocket->mass) * (planets[i]->mass)) / (r * r)) * (rocket->center.x - planets[i]->center.x));
			grav.y += (-G * (((rocket->mass) * (planets[i]->mass)) / (r * r)) * (rocket->center.y - planets[i]->center.y));
		}
	}
	rocket->gravity = grav;
}

void Physics::motorImpulse(float dt)
{
		rocket->impulse.x = rocket->impulseForce.x * rocket->mass;
		rocket->impulse.y = rocket->impulseForce.y * rocket->mass;
}
void Physics::thirdLaw()
{
	rocket->force.x = -rocket->gravity.x;
	rocket->force.y = -rocket->gravity.y;
}
void Physics::aeroDrag()
{
	float dist = norm(rocket->center, atmos->center);
	if (dist < (rocket->radius + atmos->radius))
	{
		float S = sqrt(2 * (rocket->radius * rocket->radius));
		float Cl = 0.2;
		rocket->dragForce.x = -(0.5 * pA * (rocket->velocity.x * rocket->velocity.x) * S * Cl);
		rocket->dragForce.y = -(0.5 * pA * (rocket->velocity.y * rocket->velocity.y) * S * Cl);
	}
	else
	{
		rocket->dragForce.x = 0;
		rocket->dragForce.y = 0;
	}
}
void Physics::buoyancy()
{
	float dist = norm(rocket->center, planets[2]->center);
	if (dist < (rocket->radius + planets[2]->radius))
	{
		rocket->buo.x = (density * (rocket->gravity.x) * (3, 1415 * (rocket->radius * rocket->radius)) - (rocket->mass * rocket->gravity.x))/20000000;
		rocket->buo.y = (density * (rocket->gravity.y) * (3, 1415 * (rocket->radius * rocket->radius)) - (rocket->mass * rocket->gravity.y))/20000000;
	}
	else
	{
		rocket->buo.x = 0;
		rocket->buo.y = 0;
	}
}
void Physics::checkMoonColision()
{
	float dist = norm(rocket->center, planets[1]->center);
	if (dist < moonCrash->radius + rocket->radius && (rocket->velocity.y > -600))
	{

		crashed = true;
		printf("Crashed: %d\n", crashed);
	}
	else
	{
		crashed = false;
	}
	if (dist < moonCrash->radius + rocket->radius && (rocket->velocity.y < -600))
	{
		landed = true;
		printf("Landed: %d\n", landed);
	}
	else
	{
		crashed = false;
	}
}
void Physics::calculateForces()
{
	rocket->xForces = rocket->gravity.x + rocket->impulse.x + rocket->dragForce.x + rocket->buo.x;
	rocket->yForces = rocket->gravity.y + rocket->impulse.y + rocket->dragForce.y + rocket->buo.y;

	rocket->force = { rocket->xForces,rocket->yForces };
}
