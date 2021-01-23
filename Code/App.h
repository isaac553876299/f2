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

	Physics physicsBox;
	SDL_Texture* textures[10];

	App();

	~App();

	void Input();

	void Update();

	void Draw();
};

#endif
