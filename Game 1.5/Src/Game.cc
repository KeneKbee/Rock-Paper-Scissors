#include "Game.h"

SDL_Event Game::event;

Game::Game(const char* title, int x, int y, int w, int h, bool fullscreen)
{
	Uint32 flag = fullscreen ? SDL_WINDOW_FULLSCREEN : 0;

	SDL_Surface* icon = IMG_Load("Assets/Textures/Player/hand/hand_3.png");

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		std::cout << "\033[1;36m" << "Subsystems Initialised." << "\033[0m" << std::endl;

		if (fullscreen)
		{
			SDL_Rect display;
			SDL_GetDisplayBounds(0, &display);
			w = display.w;
			h = display.h;
		}

		window = SDL_CreateWindow(title, x, y, w, h, flag);
		SDL_SetWindowIcon(window, icon);

		if (window)
		{
			std::cout << "\033[1;33m" << "Created a window." << "\033[0m" << std::endl;
		}

		renderer = SDL_CreateRenderer(window, -1, 0);

		if (renderer)
		{
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			std::cout << "\033[1;33m" << "Created the renderer." << "\033[0m" << std::endl;
		}
		try
		{
			menuIMG = AssetLoader::CreateTexture("menu.png", renderer);
			backgroundIMG = AssetLoader::CreateTexture("background.png", renderer);
			deathIMG = AssetLoader::CreateTexture("background.png", renderer);

			player = new Player(renderer);
			shark = new Shark(renderer);

			box = new MoneyBox(renderer);

			SDL_FreeSurface(icon);
			icon = nullptr;

			screen.h = 1080;
			screen.w = 1920;
			screen.y = 0;
			screen.x = 0;

			Mix_Init(0);
			Mix_OpenAudioDevice(44100, MIX_DEFAULT_FORMAT, 2, 1024, NULL, 0);

			menuMusic = AssetLoader::CreateMusic("MainMenu.wav");
			gameMusic = AssetLoader::CreateMusic("GameAmbience.wav");
			deathMusic = AssetLoader::CreateMusic("Death.wav");
			creditsMusic = AssetLoader::CreateMusic("Credits.wav");

			intro = new Video(renderer, "Intro.mp4");
			credits = new Video(renderer, "Credits.mp4");

			renderTarget = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 1920, 1080);

			float innerWindowScale = std::min((float)w / 1920, (float)h / 1080);
			
			int innerWindowWidth = (int) (1920 * innerWindowScale);
			int innerWindowHeight = (int) (1080 * innerWindowScale);
			int innerWindowOffsetX = (w - innerWindowWidth) / 2;
			int innerWindowOffsetY = (h - innerWindowHeight) / 2;

			innerWindowPos = { innerWindowOffsetX, innerWindowOffsetY, innerWindowWidth, innerWindowHeight };

			intro->play();

			isRunning = true;
		}
		catch (std::runtime_error& e)
		{
			std::cerr << "\033[1;31m" << "Error: " << e.what() << "\033[0m" << std::endl;
			isRunning = false;
		}
	}
	else {
		isRunning = false;
	}
}

void Game::handleEvents()
{
	SDL_PollEvent(&event);

	switch (event.type)
	{
		case SDL_QUIT:
			isRunning = false;
			break;

		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
				case SDLK_ESCAPE:
					isRunning = false;
					break;

				case SDLK_RETURN:
					if (gamePart == MENU)
					{
						gamePart = GAME;
						Mix_FadeOutMusic(250);
					}
					break;

				case SDLK_LSHIFT:
					if (gamePart == MENU)
					{
						gamePart = CREDITS;
						credits->play();
						Mix_FadeOutMusic(100);
					}
					break;

				case SDLK_RSHIFT:
					if (gamePart == MENU)
					{
						gamePart = CREDITS;
						credits->play();
						Mix_FadeOutMusic(100);
					}
					break;

				default:
					break;
			}
			break;

		default:
			break;
	}
}

void Game::updateIntro()
{
	intro->update();
}

void Game::updateDeath()
{
	if (player->getAnimation() == NONE && shark->getAnimation() == R_CHECK)
	{
		shark->reset();
	}

	if (shark->getAnimation() == R_CALL)
	{
		player->show();
	}

	if (player->getAnimation() == SHOW && shark->getAnimation() != R_HIDE_2)
	{
		shark->hide2();
	}
	
	if (player->getAnimation() == PICK)
	{
		animation_cnt++;
	}

	player->update();
	shark->update();

	if (Mix_PlayingMusic())
	{
		Mix_FadeOutMusic(1000);
	}
}

void Game::updateDeathScreen()
{
	screen.x -= 4;
	screen.y -= 2;
	screen.w += 8;
	screen.h += 4;
}

void Game::updateEnding()
{
	if(!shark->isHidden() || shark->getAnimation() == R_SHOW)
	{
		if (shark->getAnimation() == R_CALL)
		{
			shark->hide2();
		}

		shark->update();
	}
	else {
		if(!box->isHidden())
		{
			box->update();
		}
		else {
			gamePart = CREDITS;
			credits->play();
			credits->update();
		}
	}
}

void Game::updateCredits()
{
	if(credits->playing())
	{
		credits->update();
	}
	else {
		isRunning = false;
	}
}

void Game::updateGameAnimations()
{
	if (player->getAnimation() == NONE && shark->getAnimation() == R_CHECK)
	{
		shark->reset();
	}

	if (shark->getAnimation() == R_SHOW && player->getAnimation() == NONE)
	{
		player->reset();
	}

	if (player->getAnimation() == WAIT && shark->getAnimation() != R_SHOW && shark->getAnimation() != R_WAIT && shark->getAnimation() != R_CALL && shark->getAnimation() != R_CUT)
	{
		shark->show();
	}
	else if (player->getAnimation() == SHOW && shark->getAnimation() != R_HIDE && shark->getAnimation() != R_PICK && shark->getAnimation() != R_NONE)
	{
		shark->hide();
	}

	if (shark->getAnimation() == R_CALL && player->getAnimation() != WAIT)
	{
		player->show();
	}

	if (player->getAnimation() == WAIT && shark->getAnimation() == R_CALL)
	{
		player->check();
		shark->check();
	}

	if (player->getAnimation() == NONE && player->isHidden() && shark->getAnimation() == R_HIDE_2 && shark->isHidden())
	{
		if (animation_cnt == 100)
		{
			player->reset();
			shark->reset();
			animation_cnt = 0;
		}
		else {
			animation_cnt++;
		}
	}
}

void Game::checkResult()
{
	if (player->getValue() == shark->getValue())
	{
		player->move();
		player->resetValue();
		player->hide();
		shark->hide2();
	}
	else if ((player->getValue() == "rock" && shark->getValue() == "paper") || (player->getValue() == "paper" && shark->getValue() == "scissors") || (player->getValue() == "scissors" && shark->getValue() == "rock"))
	{
		player->resetValue();
		player->clear();
		shark->resetValue();
		shark->clear();
		player->lose();
	}
	else if ((shark->getValue() == "rock" && player->getValue() == "paper") || (shark->getValue() == "paper" && player->getValue() == "scissors") || (shark->getValue() == "scissors" && player->getValue() == "rock"))
	{
		shark->lose();
		if (shark->getAnimation() != R_CUT)
		{
			player->move();
			player->resetValue();
			player->hide();
			shark->hide2();
		}
		else
		{
			player->clear();
			player->resetValue();
			shark->clear();
		}
	}
}

void Game::updateHandCounters()
{
	if (shark->playerRockCnt != player->rockCnt && shark->getAnimation() == R_CALL)
	{
		shark->playerRockCnt = player->rockCnt;
	}

	if (shark->playerPaperCnt != player->paperCnt && shark->getAnimation() == R_CALL)
	{
		shark->playerPaperCnt = player->paperCnt;
	}

	if (shark->playerScissorsCnt != player->scissorsCnt && shark->getAnimation() == R_CALL)
	{
		shark->playerScissorsCnt = player->scissorsCnt;
	}
}

void Game::updateGame()
{
	updateGameAnimations();

	if(shark->getAnimation() == R_CHECK && player->currentValue != "" && shark->currentValue != "")
	{
		checkResult();
	}

	player->update();
	shark->update();

	updateHandCounters();
}

void Game::updateGameType()
{
	if (intro->playing() == false && gamePart == INTRO)
	{
		gamePart = MENU;
	}
	if (player->getLifes() == 0 && player->getAnimation() != CUT && gamePart != DEATH_SCREEN && gamePart != ENDING && gamePart != CREDITS)
	{
		gamePart = DEATH;
	}
	else if (shark->getFingers() == 0 && shark->getAnimation() != R_CUT && gamePart != ENDING && gamePart != CREDITS)
	{
		gamePart = ENDING;
		Mix_FadeOutMusic(100);
	}

	if (gamePart == DEATH && animation_cnt == 100)
	{
		gamePart = DEATH_SCREEN;
		animation_cnt = 0;
	}

	if (gamePart == DEATH_SCREEN && animation_cnt == 2)
	{
		isRunning = false;
	}
}

void Game::update()
{
	updateGameType();

	if (gamePart == INTRO)
	{
		updateIntro();
	}
	else if (gamePart == GAME)
	{
		updateGame();
	}
	else if (gamePart == DEATH)
	{
		updateDeath();
	}
	else if (gamePart == DEATH_SCREEN)
	{
		updateDeathScreen();
	}
	else if (gamePart == ENDING)
	{
		updateEnding();
	}
	else if (gamePart == CREDITS)
	{
		updateCredits();
	}
}

void Game::renderObjects(SDL_Renderer* renderer)
{
	if (gamePart == INTRO)
	{
		intro->render(renderer);
	}
	else if (gamePart == MENU)
	{
		SDL_RenderCopy(renderer, menuIMG, NULL, &screen);
	}
	else if (gamePart != DEATH_SCREEN && gamePart != CREDITS && gamePart != INTRO)
	{
		SDL_RenderCopy(renderer, backgroundIMG, NULL, &screen);
	}
	else if (gamePart != CREDITS && gamePart != INTRO)
	{
		SDL_RenderCopy(renderer, deathIMG, NULL, &screen);
	}

	if (gamePart != MENU && gamePart != DEATH_SCREEN && gamePart != CREDITS && gamePart != INTRO)
	{
		shark->render(renderer);
	}

	if (gamePart != MENU && gamePart != DEATH_SCREEN && gamePart != ENDING && gamePart != CREDITS && gamePart != INTRO)
	{
		player->render(renderer);
	}

	if (gamePart == ENDING)
	{
		box->render(renderer);
	}

	if (gamePart == CREDITS && credits->playing())
	{
		credits->render(renderer);
	}
}

void Game::render()
{
	SDL_SetRenderTarget(renderer, renderTarget);
	SDL_RenderClear(renderer);

	renderObjects(renderer);

	SDL_SetRenderTarget(renderer, nullptr);

	SDL_RenderCopy(renderer, renderTarget, nullptr, &innerWindowPos);

	SDL_RenderPresent(renderer);
}

void Game::playMusic()
{
	if (gamePart == MENU && Mix_PlayingMusic() == 0)
	{
		Mix_PlayMusic(menuMusic, -1);
	}
	else if (gamePart == GAME && Mix_PlayingMusic() == 0)
	{
		Mix_PlayMusic(gameMusic, -1);
	}
	else if (gamePart == DEATH_SCREEN && Mix_PlayingMusic() == 0 && animation_cnt != 2)
	{
		Mix_PlayMusic(deathMusic, 0);
		animation_cnt++;
	}
	else if (gamePart == CREDITS && Mix_PlayingMusic() == 0 && animation_cnt != 1)
	{
		Mix_PlayMusic(creditsMusic, 0);
		animation_cnt++;
	}
}

bool Game::running()
{
	return isRunning;
}

Game::~Game()
{
	delete player;
	player = nullptr;
	delete shark;
	shark = nullptr;
	delete box;
	box = nullptr;
	SDL_DestroyWindow(window);
	window = nullptr;
	SDL_DestroyRenderer(renderer);
	renderer = nullptr;

	SDL_DestroyTexture(menuIMG);
	menuIMG = nullptr;

	SDL_DestroyTexture(backgroundIMG);
	backgroundIMG = nullptr;

	SDL_DestroyTexture(deathIMG);
	deathIMG = nullptr;

	Mix_FreeMusic(menuMusic);
	menuMusic = nullptr;

	Mix_FreeMusic(gameMusic);
	gameMusic = nullptr;

	Mix_FreeMusic(deathMusic);
	deathMusic = nullptr;

	Mix_FreeMusic(creditsMusic);
	creditsMusic = nullptr;
	
	delete intro;
	intro = nullptr;

	delete credits;
	credits = nullptr;

	SDL_Quit();

	std::cout << "\033[1;36m" << "Game cleaned." << "\033[0m" << std::endl;
}