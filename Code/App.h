#ifndef _APP_H_
#define _APP_H_

#include "defs.h"
#include "Physics.h"

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

	//=========================================================================================
	Physics physicsBox;
	//=========================================================================================

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

		//=========================================================================================
		physicsBox.Update(dt, mouse, keyboard);
		//=========================================================================================

		static char title[256];
		sprintf_s(title, 256, "fps(%d) | mouse{%d|%d|%d|%d} b1->speed(%.1f) collision(%d)",
			fps,
			mouse.x, mouse.y, mouse.stateL, mouse.stateR,
			physicsBox.b1->speed,
			physicsBox.collision);
		SDL_SetWindowTitle(window, title);
	}

	void Draw()
	{
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		//=========================================================================================
		physicsBox.Draw(renderer);
		//=========================================================================================

		SDL_RenderPresent(renderer);
	}
};

#endif
