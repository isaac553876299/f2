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
	Uint32 seconds;
	int fpsCount;
	int fps;

	//KeyStates::IDLE_0,DOWN_1,REPEAT_2,UP_3
	mouse _mouse;
	int keyboard[200];

	//=========================================================================================
	Physics physicsBox;
	//=========================================================================================

	App()
	{
		SDL_Init(SDL_INIT_VIDEO);
		SDL_CreateWindowAndRenderer(WW, WH, 0, &window, &renderer);

		_mouse = { 0,0,0,0,0,0 };

		for (auto i : keyboard) i = 0;
		for (int i = 0; i < 200; i++) keyboard[i] = 0;

		dt = SDL_GetTicks() / 1000;
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
		if (_mouse.stateL == 1) _mouse.stateL = 2;
		if (_mouse.stateR == 1) _mouse.stateR = 2;
		if (_mouse.stateL == 3) _mouse.stateL = 0;
		if (_mouse.stateR == 3) _mouse.stateR = 0;

		SDL_GetMouseState(&_mouse.x, &_mouse.y);

		SDL_Event event;
		while (SDL_PollEvent(&event))
			switch (event.type)
			{
			case SDL_QUIT: quit = true; break;
			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_LEFT) _mouse.stateL = 1;
				if (event.button.button == SDL_BUTTON_RIGHT) _mouse.stateR = 1;
				break;
			case SDL_MOUSEBUTTONUP:
				if (event.button.button == SDL_BUTTON_LEFT) _mouse.stateL = 3;
				if (event.button.button == SDL_BUTTON_RIGHT) _mouse.stateR = 3;
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
		dt = (SDL_GetTicks() / 1000) - dt;

		fpsCount++;
		if (SDL_GetTicks() - seconds > 1000)
		{
			seconds = SDL_GetTicks();
			fps = fpsCount;
			fpsCount = 0;
		}

		//=========================================================================================
		physicsBox.Input(_mouse, keyboard);
		physicsBox.Update(dt);
		//=========================================================================================

		static char title[256];
		sprintf_s(title, 256, "fps(%d) | mouse{%d,%d,%d,%d}",
			fps, _mouse.x, _mouse.y, _mouse.stateL, _mouse.stateR);
		SDL_SetWindowTitle(window, title);

		//Uint32 _dt = SDL_GetTicks() - 1000 * dt;
		//Uint32 delay = ((1000 / FPS) - _dt > 0) ? ((1000 / FPS) - _dt) : 0;
		//SDL_Delay(delay);
		// (edt - dt > 0)(dt < edt)
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
