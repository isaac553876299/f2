#include "defs.h"
#include "App.h"

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
