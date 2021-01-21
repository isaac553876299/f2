#include <iostream>
#include "../SDL2-2.0.14/include/SDL.h"

//=================================================================================================

//http://geomalgorithms.com/a03-_inclusion.html

#define FPS 60
#define _dt (1000/FPS)

#define RELEASE(x) { delete x; x = nullptr; }
#define RELEASE_ARRAY(x) { delete[] x; x = nullptr; }

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

#define RAD(a) (a*3.14/180)

struct Mouse { int x, y, offsx, offsy, stateL, stateR; };

struct fPoint { float x, y; };

void DrawfLine(SDL_Renderer* renderer, fPoint p0, fPoint p1)
{
	SDL_RenderDrawLine(renderer, p0.x, p0.y, p1.x, p1.y);
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

/*
template<class tdata>
struct ListItem
{
	tdata data;
	ListItem<tdata>* next = nullptr;
	ListItem<tdata>* prev = nullptr;

	inline ListItem(const tdata& _data) : data(_data) {}
	~ListItem() {}
};

template<class tdata>
class List
{
public:
	ListItem<tdata>* start;
	ListItem<tdata>* end;
private:
	unsigned int size;
public:
	List() { init(); }
	~List() { clear(); }

	void init() { start = end = nullptr; size = 0; }

	unsigned int Count() const { return size; }

	ListItem<tdata>* add(const tdata& item)
	{
		ListItem<tdata>* dataItem = new ListItem<tdata>(item);

		end->next = dataItem;
		dataItem->prev = end;
		end = dataItem;
		dataItem->next = start;
		start->prev = dataItem;

		++size;
		return dataItem;
	}

	void del(ListItem<tdata>* item)
	{
		if (item != nullptr)
		{
			if (item->prev != nullptr)
			{
				item->prev->next = item->next;
				if (item->next != nullptr) item->next->prev = item->prev;
				else end = item->prev;
			}
			else
			{
				if (item->next != nullptr)
				{
					item->next->prev = nullptr;
					start = item->next;
				}
				else start = end = nullptr;
			}

			RELEASE(item);
			--size;
		}
	}

	void clear()
	{
		while (start != nullptr)
		{
			ListItem<tdata>* p_next = start->next;
			RELEASE(start);
			start = p_next;
		}
		init();
	}

	tdata& operator [](const unsigned int index)
	{
		for (ListItem<tdata>* p_item = start, long pos = 0;
			p_item != nullptr && pos < index;
			p_item = p_item->next; pos++);
		return p_item->data;
	}

	const tdata& operator [](const unsigned int index) const
	{
		for (ListItem<tdata>* p_item = start, long pos = 0;
			p_item != nullptr && pos < index;
			p_item = p_item->next; pos++);
		return (p_item->data != nullptr) ? p_item->data : nullptr;//assert
	}
};*/

//=================================================================================================

class Body
{
public:
	int nsides;
	fPoint center;
	float radius;
	fPoint* vertex;

	float directionAngle;
	float rotationAngle;
	float velocity;
	float acceleration;
	float force;
	float mass;

	Body(int n, float x, float y, float r, float da, float ra, float v, float a, float f, float m)
	{
		nsides = n;
		center = { x,y };
		radius = r;
		vertex = new fPoint[nsides];
		UpdateVertex();
		velocity = v;
		directionAngle = da;
		rotationAngle = ra;
		
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

	void Draw(SDL_Renderer* renderer)
	{
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		for (int i = 0; i < nsides - 1; i++)
			DrawfLine(renderer, vertex[i], vertex[i + 1]);
		DrawfLine(renderer, vertex[nsides - 1], vertex[0]);

		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		fPoint p2{ center.x + 100 * cos(RAD(directionAngle)),center.y + 100 * sin(RAD(directionAngle)) };
		DrawfLine(renderer, center, p2);
	}

};

class Physics
{
public:

	Body* b1 = new Body(10, 100.f, 100.f, 50.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f);

	Physics() {};
	~Physics() {};

	void Input(Mouse _mouse, int* _keyboard)
	{
		if (_keyboard[SDL_SCANCODE_1]) b1->velocity += 0.1f;
		if (_keyboard[SDL_SCANCODE_2]) b1->velocity -= 0.1f;
		if (_keyboard[SDL_SCANCODE_3]) b1->acceleration += 0.1f;
		if (_keyboard[SDL_SCANCODE_4]) b1->acceleration -= 0.1f;

		if (_keyboard[SDL_SCANCODE_R]) b1->center = { WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 };
		if (_keyboard[SDL_SCANCODE_T]) b1->velocity = 0.f;


		float increment = 1.0f;
		if (_keyboard[SDL_SCANCODE_C]) b1->directionAngle -= increment;
		if (_keyboard[SDL_SCANCODE_V]) b1->directionAngle += increment;
		if (_keyboard[SDL_SCANCODE_Z]) b1->rotationAngle -= increment;
		if (_keyboard[SDL_SCANCODE_X]) b1->rotationAngle += increment;
	}

	void Update(float dt)//step
	{
		int i = 0;
		switch (i)
		{
		case 0://Implicit Euler
			b1->center.x += b1->velocity * cos(RAD(b1->directionAngle)) * dt;
			b1->center.y += b1->velocity * sin(RAD(b1->directionAngle)) * dt;
			b1->velocity += b1->acceleration * dt;
			break;
		case 1://Symplectic Euler
			b1->velocity += b1->acceleration * dt;
			b1->center.x += b1->velocity * cos(RAD(b1->directionAngle)) * dt;
			b1->center.y += b1->velocity * sin(RAD(b1->directionAngle)) * dt;
			break;
		case 2://Velocity-Verlet
			
			break;
		case 3://Störmer-Verlet.

			break;
		default:
			break;
		}

		b1->UpdateVertex();
	}

	void Draw(SDL_Renderer* renderer)
	{
		b1->Draw(renderer);

	}

	void Collide(Body* b0, Body* b1)
	{

	}

	void OnCollision()
	{

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
		sprintf_s(title, 256, "fps(%d) dt(%.4f) | mouse{%d|%d|%d|%d}",
			fps, dt,
			mouse.x, mouse.y, mouse.stateL, mouse.stateR);
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
