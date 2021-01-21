#include <iostream>
#include "../SDL2-2.0.14/include/SDL.h"

//=================================================================================================

//http://geomalgorithms.com/a03-_inclusion.html

#define FPS 60
#define _dt (1000/FPS)/1000
//dt_30_60_120_144{0.0333,0.0167,0.0083,0.00694}

#define RELEASE(x) { delete x; x = nullptr; }
#define RELEASE_ARRAY(x) { delete[] x; x = nullptr; }

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

#define RAD(a) (a*3.14/180)

struct Mouse { int x, y, offsx, offsy, stateL, stateR; };

struct fPoint { float x, y; };

struct vertexbool
{
	fPoint v;
	bool in;
};

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
	/*inline*/Uint32 msRead() { return time; };
	/*inline*/float sRead() { return float(time / 1000); };
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

		dataItem->next = start;
		start->prev = dataItem;
		start = dataItem;

		dataItem->prev = end;
		end->next = dataItem;
		end = dataItem;

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

	fPoint velocity;
	float directionAngle;
	float rotationAngle;

	Body(int n, float x, float y, float r, float vx, float vy, float da, float ra)
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
	~Body() {};

	void Draw(SDL_Renderer* renderer)
	{
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		for (int i = 0; i < sizeof(vertex) - 1; i++)
			DrawfLine(renderer, vertex[i], vertex[i + 1]);
		DrawfLine(renderer, vertex[sizeof(vertex) - 1], vertex[0]);

		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		fPoint p2{ center.x + 100 * cos(RAD(directionAngle)),center.y + 100 * sin(RAD(directionAngle)) };
		DrawfLine(renderer, center, p2);
	}

};

class Physics
{
public:

	Body* b1 = new Body(4, 100.f, 100.f, 50.f, 0.f, 0.f, 0.f, 0.f);

	Physics() {};
	~Physics() {};

	void Input(Mouse _mouse, int* _keyboard)
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

	void Update(float dt)
	{
		//=====FIX=====
		dt = 0.006944f;
		//=====FIX=====


	}

	void Draw(SDL_Renderer* renderer)
	{
		b1->Draw(renderer);

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
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED/*);//*/ | SDL_RENDERER_PRESENTVSYNC);

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
		sprintf_s(title, 256, "fps(%d) | mouse{%d|%d|%d|%d} b1{velocity{%.1f,%.1f},directionAngle{%.1f},rotationAngle{%.1f}",
			fps,
			mouse.x, mouse.y, mouse.stateL, mouse.stateR,
			physicsBox.b1->velocity.x, physicsBox.b1->velocity.y, physicsBox.b1->directionAngle, physicsBox.b1->rotationAngle);
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
