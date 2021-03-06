#include "Physics.h"

Physics::Physics()
{
	//TTF_Font* font = TTF_OpenFont("digital7regular.ttf", 25);


	rocket = new Body(7, PIXEL_TO_METERS(18000.f), PIXEL_TO_METERS(25000.f), PIXEL_TO_METERS(2500.0f), 0.f, 0.f, 20000.0f);
	//earth
	planets[0] = new Body(50, PIXEL_TO_METERS(18000.f), PIXEL_TO_METERS(70000.f), PIXEL_TO_METERS(30000.0f), 0.f, 0.f, 500000.0f);
	//moon
	planets[1] = new Body(50, PIXEL_TO_METERS(18000.f), PIXEL_TO_METERS(-180000.f), PIXEL_TO_METERS(15000.0f), 0.f, 0.f, 300000.0f);
	//win condition
	wincon = new Body(50, PIXEL_TO_METERS(18000.f), PIXEL_TO_METERS(-200000.f), PIXEL_TO_METERS(1000.0f), 0.f, 0.f, 0.0f);
	//fluid
	planets[2] = new Body(25, PIXEL_TO_METERS(18000.f), PIXEL_TO_METERS(10000), PIXEL_TO_METERS(10000.0f), 0.f, 0.f, 10000.0f);
	//atmosphere
	atmos = new Body(50, PIXEL_TO_METERS(18000.f), PIXEL_TO_METERS(70000.f), PIXEL_TO_METERS(50000.0f), 0.f, 0.f, 0.0f);
	//rocks
	planets[3] = new Body(8, PIXEL_TO_METERS(-45000.f), PIXEL_TO_METERS(-130000), PIXEL_TO_METERS(2000.0f), 0.f, 0.f, 0.0f);
	planets[4] = new Body(8, PIXEL_TO_METERS(-30000.f), PIXEL_TO_METERS(-100000), PIXEL_TO_METERS(2000.0f), 0.f, 0.f, 0.0f);
	planets[5] = new Body(8, PIXEL_TO_METERS(-15000.0f), PIXEL_TO_METERS(-150000), PIXEL_TO_METERS(2000.0f), 0.f, 0.f, 0.0f);
	planets[6] = new Body(8, PIXEL_TO_METERS(0.0f), PIXEL_TO_METERS(-120000), PIXEL_TO_METERS(2000.0f), 0.f, 0.f, 0.0f);
	planets[7] = new Body(8, PIXEL_TO_METERS(15000.0f), PIXEL_TO_METERS(-110000), PIXEL_TO_METERS(2000.0f), 0.f, 0.f, 0.0f);
	planets[8] = new Body(8, PIXEL_TO_METERS(30000.f), PIXEL_TO_METERS(-90000), PIXEL_TO_METERS(2000.0f), 0.f, 0.f, 0.0f);
	planets[9] = new Body(8, PIXEL_TO_METERS(45000.f), PIXEL_TO_METERS(-100000), PIXEL_TO_METERS(2000.0f), 0.f, 0.f, 0.0f);
	planets[10] = new Body(8, PIXEL_TO_METERS(60000.f), PIXEL_TO_METERS(-110000), PIXEL_TO_METERS(2000.0f), 0.f, 0.f, 0.0f);
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
	case 3://St�rmer-Verlet.

		break;
	default:
		break;
	}

	
	rocket->UpdateVertex();

	Collide();

	camera.x = rocket->center.x - (WINDOW_WIDTH / 2);
	camera.y = rocket->center.y - (WINDOW_HEIGHT / 2);
}

void Physics::Draw(SDL_Renderer* renderer)
{
	rocket->Draw(renderer, camera, debugCollisions);
	for (int i = 0; i < 20; i++)
	{
		if (planets[i] != nullptr)
		{
			planets[i]->Draw(renderer, camera, debugCollisions);
			SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
			if(i<2)
				DrawfLine(renderer, camera, rocket->center, planets[i]->center);
		}

	}

	atmos->Draw(renderer, camera, debugCollisions);
	wincon->Draw(renderer, camera, debugCollisions);

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
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	for (int i = 3; i < 11; i++)
	{
		float prop = WINDOW_HEIGHT * abs(planets[1]->center.y - planets[i]->center.y) / abs(planets[1]->center.y - planets[0]->center.y);
		SDL_Rect pos1 = { 50,prop,10,10 };
		SDL_RenderFillRect(renderer, &pos1);
	}


	SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
	float velR = norm(rocket->velocity) / 7;
	fPoint p1{ rocket->center.x + velR * cos(RAD(rocket->directionAngle) + RAD(180) + RAD(0)),rocket->center.y + velR * sin(RAD(rocket->directionAngle) + RAD(180) + RAD(0)) };
	fPoint p2{ rocket->center.x + velR * cos(RAD(rocket->directionAngle) + RAD(180) + RAD(10)),rocket->center.y + velR * sin(RAD(rocket->directionAngle) + RAD(180) + RAD(10)) };
	fPoint p3{ rocket->center.x + velR * cos(RAD(rocket->directionAngle) + RAD(180) - RAD(10)),rocket->center.y + velR * sin(RAD(rocket->directionAngle) + RAD(180) - RAD(10)) };
	DrawfLine(renderer, camera, rocket->center, p1);
	DrawfLine(renderer, camera, rocket->center, p2);
	DrawfLine(renderer, camera, rocket->center, p3);


	//SDL_Rect tex = { (rocket->center.x - camera.x - rocket->radius),(rocket->center.y - camera.y - rocket->radius), 2 * rocket->radius, 2 * rocket->radius };
	//SDL_RenderCopy(renderer, textures[0], 0, &tex);


	//SDL_Color color = { 255, 255, 255 };
	//SDL_Surface* surface = TTF_RenderText_Solid(font, "Welcome to Gigi Labs", { 255,255,0 });
	//SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	//SDL_RenderCopy(renderer, texture, NULL, NULL);

	/*SDL_Surface* sur = IMG_Load("Output/Textures/rocket.png");
	texture = SDL_CreateTextureFromSurface(renderer, sur);
	SDL_RenderCopy(renderer, texture, NULL, NULL);*/

	SDL_RenderPresent(renderer);
}

void Physics::Collide()
{
	for (int i = 0; i < 20; i++)
	{
		if (rocket != nullptr && planets[i] != nullptr && i!=2)
		{
			float dist = norm(rocket->center, planets[i]->center);
			if (dist < (rocket->radius + planets[i]->radius))
			{
				if (!moonLanded && !spaced)
				{
					rocket->velocity = { 0.f,0.f };
				}
				float vx = (rocket->center.x - planets[i]->center.x) / dist;
				float vy = (rocket->center.y - planets[i]->center.y) / dist;
				rocket->center.x = (planets[i]->center.x + (planets[i]->radius + rocket->radius) * cos(acos(vx)));
				rocket->center.y = (planets[i]->center.y + (planets[i]->radius + rocket->radius) * sin(asin(vy)));

				if (i == 1 && ((norm(rocket->velocity)) > 600))
				{
					reset();
					system("cls");
					printf("Crash!\n");
				}
				float dist3 = norm(rocket->center, wincon->center);
				if (dist3 < (rocket->radius + wincon->radius))
				{
					reset();
					system("cls");
					printf("Win!\n");
				}
				if (i > 2)
				{
					reset();
					system("cls");
					printf("Crashed!\n");
				}
			}
		}
	}
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
void Physics::calculateForces()
{
	rocket->xForces = rocket->gravity.x + rocket->impulse.x + rocket->dragForce.x + rocket->buo.x;
	rocket->yForces = rocket->gravity.y + rocket->impulse.y + rocket->dragForce.y + rocket->buo.y;

	rocket->force = { rocket->xForces,rocket->yForces };
}
void Physics::reset()
{
	rocket->center.x = PIXEL_TO_METERS(18000);
	rocket->center.y = PIXEL_TO_METERS(30000);
	rocket->velocity = { 0,0 };
	rocket->acceleration = { 0,0 };
	rocket->force = { 0,0 };

}
