#include "App.h"
SDL_Texture* playerTex;
const SDL_Rect* lol = { };
App::App()
{
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);
	window = SDL_CreateWindow("Physics Box", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);//*/ | SDL_RENDERER_PRESENTVSYNC);

	mouse = { 0,0,0,0,0,0 };

	for (auto i : keyboard) i = 0;
	for (int i = 0; i < 200; i++) keyboard[i] = 0;

	seconds = SDL_GetTicks();
	fpsCount = 0;
	fps = 0;

	SDL_Surface* tmpSurface = IMG_Load("Output/Textures/rocket.png");
	playerTex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
	SDL_FreeSurface(tmpSurface);
	textures[0] = IMG_LoadTexture(renderer, "Output/Textures/rocket.png");
}

App::~App()
{
	SDL_Quit();
}

void App::Input()
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

void App::Update()
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
	sprintf_s(title, 256, "fps(%d) dt(%.4f) | mouse{%d|%d|%d|%d|	PLAYER: %.f %.f | GRAVITY X: %.2f  GRAVITY Y: %.2f} | VELOCITY: x%.2f y%.2f | DRAG FORCE X: %.f DRAG FORCE Y: %.f",
		fps, dt,
		mouse.x, mouse.y, mouse.stateL, mouse.stateR,
		physicsBox.rocket->center.x, physicsBox.rocket->center.y,
		physicsBox.rocket->gravity.x, physicsBox.rocket->gravity.y,
		physicsBox.rocket->velocity.x, physicsBox.rocket->velocity.y,
		physicsBox.rocket->dragForce.x, physicsBox.rocket->velocity.y);
	SDL_SetWindowTitle(window, title);
}

void App::Draw()
{
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	physicsBox.Draw(renderer, textures);

	SDL_RenderCopy(renderer, playerTex, NULL,NULL);

	SDL_RenderPresent(renderer);
}

