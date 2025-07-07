#include "Game.h"

#ifdef _WIN32
	#include "WinAnsi.h"
#endif

int main(int argc, char* argv[])
{

	#ifdef WINANSI_H
		win::enableAnsi();
	#endif

	const int FPS = 60;
	const int frameDelay = 1000 / FPS;

	Uint32 frameStart;
	int frameTime;

	Game* game = new Game("Rock-Paper-Scissors", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1920, 1080, 0);
	
	while (game->running())
	{
		frameStart = SDL_GetTicks();

		game->handleEvents();
		game->update();
		game->render();
		game->playMusic();

		frameTime = SDL_GetTicks() - frameStart;

		if (frameDelay > frameTime)
		{
			SDL_Delay(frameDelay - frameTime);
		}
	}

	delete game;
	game = nullptr;

	std::cout << "\nPress Enter to close this console... ";
	std::cin.get();

	return 0;
}
