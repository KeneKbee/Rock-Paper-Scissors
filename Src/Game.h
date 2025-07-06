#pragma once

#ifndef GAME_H
#define GAME_H

#include "Classes/AssetLoader.h"
#include "Classes/Player.h"
#include "Classes/Shark.h"
#include "Classes/Video.h"
#include "Classes/MoneyBox.h"

// Parts of game
#define MENU			0
#define GAME			1
#define DEATH			2
#define DEATH_SCREEN	3
#define ENDING			4
#define CREDITS			5

class Game
{
public:
	Game(const char* title, int x, int y, int w, int h, bool fullscreen);
	void handleEvents();
	void update();
	void render();
	void playMusic();
	bool running();
	~Game();
private:
	void updateDeath();
	void updateDeathScreen();
	void updateEnding();
	void updateCredits();
	void updateGameAnimations();
	void checkResult();
	void updateHandCounters();
	void updateGame();
	void updateGameType();
public:
	static SDL_Event event;
private:
	bool isRunning;
	int gamePart = MENU;
	int animation_cnt = 0;
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	Player* player = nullptr;
	Shark* shark = nullptr;

	MoneyBox* box = nullptr;

	Mix_Music* menuMusic = nullptr;
	Mix_Music* gameMusic = nullptr;
	Mix_Music* deathMusic = nullptr;
	Mix_Music* creditsMusic = nullptr;

	SDL_Texture* menuIMG = nullptr;
	SDL_Texture* backgroundIMG = nullptr;
	SDL_Texture* deathIMG = nullptr;
	SDL_Rect screen;

	Video* credits = nullptr;
};

#endif

