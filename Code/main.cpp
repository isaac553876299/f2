#include <iostream>
#include <cmath>
#include "../SDL2-2.0.14/include/SDL.h"

//=================================================================================================

//http://geomalgorithms.com/a03-_inclusion.html

#define FPS 60
#define _dt (1000/FPS)

#define RELEASE(x) { delete x; x = nullptr; }
#define RELEASE_ARRAY(x) { delete[] x; x = nullptr; }

#define WINDOW_WIDTH 720
#define WINDOW_HEIGHT 920

#define GRAVITYEARTH 9.8
#define GRAVITYMOON 1.6

#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

#define G pow(10,-2)*6.6

#define RAD(a) (a*3.14/180)

struct Mouse { int x, y, offsx, offsy, stateL, stateR; };

struct fPoint { float x, y; };

void DrawfLine(SDL_Renderer* renderer, fPoint camera, fPoint p0, fPoint p1)
{
	SDL_RenderDrawLine(renderer, p0.x - camera.x, p0.y - camera.y, p1.x - camera.x, p1.y - camera.y);
}

struct fRect { float x, y, w, h; };

bool PointInRect(SDL_Point p, SDL_Rect r) { return (p.x > r.x && p.x < r.x + r.w && p.y > r.y && p.y < r.y + r.h); }
bool fPointInRect(fPoint p, SDL_Rect r) { return (p.x > r.x && p.x < r.x + r.w && p.y > r.y && p.y < r.y + r.h); }

float norm(float x0, float y0, float x1, float y1)
{
	fPoint v{ (x0 - x1),(y0 - y1) };
	return (float(sqrt((v.x * v.x) + (v.y * v.y))));
}
float norm(fPoint p0, fPoint p1)
{
	fPoint v{ (p0.x - p1.x),(p0.y - p1.y) };
	return (float(sqrt((v.x * v.x) + (v.y * v.y))));
}
float norm(fPoint v) { return (float(sqrt((v.x * v.x) + (v.y * v.y)))); }

struct Timer
{
	Uint32 time;
	Timer() { Start(); };
	void Start() { time = SDL_GetTicks(); };
	/*inline*/Uint32 msRead() { return (SDL_GetTicks() - time); };
	/*inline*/float sRead() { return float((SDL_GetTicks() - time) / 1000.f); };
};

class Body
{
public:
	int nsides;
	fPoint center;
	float radius;
	fPoint* vertex;

	float directionAngle;
	float rotationAngle;
	fPoint velocity;
	fPoint acceleration;
	fPoint force;
	float mass;
	fPoint gravity;

	Body(int n, float x, float y, float r, float da, float ra, float m)
	{
		nsides = n;
		center = { x,y };
		radius = r;
		vertex = new fPoint[nsides];
		UpdateVertex();
		directionAngle = da;
		rotationAngle = ra;
		mass = m;
		
	}
	~Body() {};

	void UpdateVertex()
	{
		for (int i = 0; i < nsides; i++)
		{
			vertex[i].x = center.x + radius * cos(RAD((360 / nsides) * i));
			vertex[i].y = center.y + radius * sin(RAD((360 / nsides) * i));
		}
	}

	void Draw(SDL_Renderer* renderer, fPoint camera)
	{
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		for (int i = 0; i < nsides - 1; i++)
			DrawfLine(renderer, camera, vertex[i], vertex[i + 1]);
		DrawfLine(renderer, camera, vertex[nsides - 1], vertex[0]);

		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		fPoint p2{ center.x + 100 * cos(RAD(directionAngle)),center.y + 100 * sin(RAD(directionAngle)) };
		DrawfLine(renderer, camera, center, p2);
	}

};

class Physics
{
public:
	Body* rocket = new Body(4, PIXEL_TO_METERS(18000.f), PIXEL_TO_METERS(25000.f), PIXEL_TO_METERS(2500.0f), 0.f,0.f, 10000.0f);
	Body* earth = new Body(50, PIXEL_TO_METERS(18000.f), PIXEL_TO_METERS(70000.f), PIXEL_TO_METERS(30000.0f), 0.f,0.f, 500000.0f);

	fPoint camera{ 0.f,0.f };

	Physics() {};
	~Physics() {};

	void Input(Mouse _mouse, int* _keyboard)
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

	void Update(float dt)//step
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
			rocket->center.x += (rocket->velocity.x *dt) + (0.5f*rocket->acceleration.x)*(dt*dt);
			rocket->center.y += (rocket->velocity.y * dt) + (0.5f*rocket->acceleration.y*(dt*dt));
			break;
		case 3://Störmer-Verlet.

			break;
		default:
			break;
		}

		rocket->UpdateVertex();
	}

	void Draw(SDL_Renderer* renderer)
	{
		SDL_Rect defaultCamera{ camera.x,camera.y,WINDOW_WIDTH,WINDOW_HEIGHT };
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
		SDL_RenderDrawRect(renderer, &defaultCamera);
		rocket->Draw(renderer, camera);
		earth->Draw(renderer, camera);

	}

	void Collide(Body* b0, Body* b1)
	{

	}

	void OnCollision()
	{

	}
	void secondLaw()
	{
		//rocket->force = rocket->mass * rocket->acceleration;
	}
	void gravity()
	{
		//if (onEarth)
		{
			float r = norm(rocket->center, earth->center);
			rocket->gravity.x = (-G * (((rocket->mass) * (earth->mass)) / (r * r)) * (rocket->center.x - earth->center.x));
			rocket->gravity.y = -G * (((rocket->mass) * (earth->mass)) / (r * r)) * (rocket->center.y-earth->center.y);

			rocket->force = { rocket->gravity.x, rocket->gravity.y };

		}

	}
};

//=================================================================================================

class App
{
public:
	bool quit = false;
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;

	float dt;
	Timer timer;

	Uint32 seconds;
	int fpsCount;
	int fps;

	//KeyStates::IDLE_0,DOWN_1,REPEAT_2,UP_3
	Mouse mouse;
	int keyboard[200];

	Physics physicsBox;

	App()
	{
		SDL_Init(SDL_INIT_VIDEO);
		window = SDL_CreateWindow("Physics Box", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);//*/ | SDL_RENDERER_PRESENTVSYNC);

		mouse = { 0,0,0,0,0,0 };

		for (auto i : keyboard) i = 0;
		for (int i = 0; i < 200; i++) keyboard[i] = 0;

		seconds = SDL_GetTicks();
		fpsCount = 0;
		fps = 0;
	}

	~App()
	{
		SDL_Quit();
	}

	void Input()
	{
		if (mouse.stateL == 1) mouse.stateL = 2;
		if (mouse.stateR == 1) mouse.stateR = 2;
		if (mouse.stateL == 3) mouse.stateL = 0;
		if (mouse.stateR == 3) mouse.stateR = 0;

		SDL_GetMouseState(&mouse.x, &mouse.y);

		SDL_Event event;
		while (SDL_PollEvent(&event))
			switch (event.type)
			{
			case SDL_QUIT: quit = true; break;
			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_LEFT) mouse.stateL = 1;
				if (event.button.button == SDL_BUTTON_RIGHT) mouse.stateR = 1;
				break;
			case SDL_MOUSEBUTTONUP:
				if (event.button.button == SDL_BUTTON_LEFT) mouse.stateL = 3;
				if (event.button.button == SDL_BUTTON_RIGHT) mouse.stateR = 3;
				break;
			}

		const Uint8* keyboardState = SDL_GetKeyboardState(0);
		if (keyboardState[SDL_SCANCODE_ESCAPE]) quit = true;

		for (int i = 0; i < 200; i++)
		{
			if (keyboard[i] == 3) keyboard[i] = 0;
			if (keyboardState[i])
			{
				if (keyboard[i] == 0 || keyboard[i] == 3) keyboard[i] = 1;
				if (keyboard[i] == 1) keyboard[i] = 2;
			}
			else if (keyboard[i] != 0) keyboard[i] = 3;
		}
	}

	void Update()
	{
		dt = timer.sRead();
		timer.Start();
		if (dt < _dt) SDL_Delay(_dt - dt);
		//if (_dt - dt > 0) SDL_Delay(_dt - dt);

		fpsCount++;
		if (SDL_GetTicks() - seconds > 1000)
		{
			seconds = SDL_GetTicks();
			fps = fpsCount;
			fpsCount = 0;
		}

		physicsBox.Input(mouse, keyboard);
		physicsBox.Update(dt);

		static char title[256];
		sprintf_s(title, 256, "fps(%d) dt(%.4f) | mouse{%d|%d|%d|%d|	PLAYER: %.f %.f}",
			fps, dt,
			mouse.x, mouse.y, mouse.stateL, mouse.stateR,physicsBox.rocket->center.x,physicsBox.rocket->center.y);
		SDL_SetWindowTitle(window, title);
	}

	void Draw()
	{
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		physicsBox.Draw(renderer);

		SDL_RenderPresent(renderer);
	}
};

//=================================================================================================

int main(int argc, char** argv)
{
	App* game = new App;
	while (!game->quit)
	{
		game->Input();
		game->Update();
		game->Draw();
	}
	delete game;
	game = nullptr;
	return 0;
}
